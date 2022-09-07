// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"

#include <errno.h>
#include <float.h>
#include <math.h>
#include <string.h>

static int solve(const float *const LU, float *x, float *b, uint8_t *P, uint16_t row)
{
	// forward substitution with pivoting
	for (int i = 0; i < row; ++i) {
		x[i] = b[P[i]];

		for (int j = 0; j < i; ++j) {
			x[i] = x[i] - LU[row * P[i] + j] * x[j];
		}
	}

	// backward substitution with pivoting
	for (int i = row - 1; i >= 0; --i) {
		for (int j = i + 1; j < row; ++j) {
			x[i] = x[i] - LU[row * P[i] + j] * x[j];
		}

		// Just in case if we divide with zero
		if (fabsf(LU[row * P[i] + i]) > FLT_EPSILON) {
			x[i] = x[i] / LU[row * P[i] + i];
		} else {
			return -ENOTSUP;
		}
	}

	return 0;
}

int inv(float *Ai_out, const float *const A, uint16_t row)
{
	// Create temporary matrix and status variable
	float tmpvec[row];
	float Ai[row * row];

	memset(tmpvec, 0, row * sizeof(float));

	// Check if the determinant is 0
	float LU[row * row];
	uint8_t P[row];

	if (lup(A, LU, P, row) != 0) {
		return -ENOTSUP;
	}

	// Create the inverse
	for (uint16_t i = 0; i < row; i++) {
		tmpvec[i] = 1.0f;
		if (solve(LU, &Ai[row * i], tmpvec, P, row) != 0) {
			return -ENOTSUP; // We divided with zero
		}
		tmpvec[i] = 0.0f;
	}

	// Transpose result
	tran(Ai, Ai, row, row);

	memcpy(Ai_out, Ai, sizeof(float) * row * row);

	return 0;
}

/*
 * GNU Octave code:
 *   >> A = [3 4 5; 2 5 6; 5 6 7];
	 >> inv(A)
	 ans =

	 0.25000  -0.50000   0.25000
	 -4.00000   1.00000   2.00000
	 3.25000  -0.50000  -1.75000

	 >>
 */
