// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"

void pinv(float *Ai, const float *const A, uint16_t row, uint16_t column)
{
	// Use Golub and Reinch if row != column
	float U[row * column];
	float S[column];
	float V[column * column];

	if (row == column)
		svd_jacobi_one_sided(A, row, MAX_ITERATION_COUNT_SVD, U, S, V);
	else
		svd_golub_reinsch(A, row, column, U, S, V);

	// Do inv(S)
	for (uint16_t i = 0; i < column; i++) {
		// Create inverse diagonal matrix
		// TODO: can this ever result in div by zero?
		S[i] = 1.0f / S[i];
	}

	// Transpose U'
	tran(U, U, row, column);

	// U = S*U'
	for (uint16_t i = 0; i < row; i++) {
		for (uint16_t j = 0; j < column; j++) {
			U[row * j + i] = S[j] * U[row * j + i];
		}
	}

	// Do pinv now: A = V*U
	mul(Ai, V, U, column, column, column, row);
}
