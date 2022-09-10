// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"
#include "control/misc.h"
#include "control/sysid.h"

#include <errno.h>
#include <math.h>

int okid_era(float *A, float *B, float *C, uint8_t row_a, const float *const y,
	     const float *const u, uint16_t io_row, uint16_t io_column)
{
	if ((io_row == 0) || (io_column == 0)) {
		return -EINVAL;
	}
	if (row_a == 0) {
		return -EINVAL;
	}

	// Markov parameters - Impulse response
	float g[io_row * io_column];

	// g = y / u (done in matrix form)
	linsolve_markov(g, y, u, io_row, io_column);

	// Compute the correct dimensions for matrix H
	const uint16_t row_h = io_row * (io_column / 2);
	const uint16_t column_h = io_column / 2;

	float Temp[row_h * column_h]; // Temporary

	// Create Half Hankel matrix
	float H[row_h * column_h];

	// Need to have 1 shift for this algorithm
	hankel(g, H, io_row, io_column, row_h, column_h, 1);

	// Do SVD on the half hankel matrix H
	float U[row_h * column_h];
	float S[column_h];
	float V[column_h * column_h];

	svd_golub_reinsch(H, row_h, column_h, U, S, V);

	// Re-create another hankel with shift = 2
	hankel(g, H, io_row, io_column, row_h, column_h,
	       2); // Need to have 2 shift for this algorithm

	// Create C and B matrix
	for (int i = 0; i < row_a; i++) {
		for (int j = 0; j < io_row; j++) {
			// C = U*S^(1/2)
			C[j * row_a + i] = U[j * column_h + i] * sqrtf(S[i]);
		}

		for (int j = 0; j < io_row; j++) {
			// B = S^(1/2)*V^T
			B[i * io_row + j] = sqrtf(S[i]) * V[j * column_h + i];
		}
	}

	// A = S^(-1/2)*U^T*H*V*S^(-1/2)

	// V = V*S^(-1/2)
	for (int i = 0; i < column_h; i++) {
		for (int j = 0; j < column_h; j++) {
			V[j * column_h + i] *= sqrtf(1.0f / S[i]);
		}
	}

	// U = S^(-1/2)*U^T
	tran(U, U, row_h, column_h);
	for (int i = 0; i < row_h; i++) {
		for (int j = 0; j < column_h; j++) {
			U[j * row_h + i] *= sqrtf(1.0f / S[j]);
		}
	}

	// Create A matrix: T = H*V
	mul(Temp, H, V, row_h, column_h, column_h, column_h);

	// Now, multiply V = U(column_h, row_h)*Temp(row_h, column_h). U is transpose!
	mul(V, U, Temp, column_h, row_h, row_h, column_h);

	// Get the elements of V -> A
	cut(A, V, column_h, column_h, 0, 0, row_a, row_a);

	return 0;
}
