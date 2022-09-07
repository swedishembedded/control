// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"

#include <float.h>
#include <math.h>
#include <string.h>

/*
 * Create A = L*L^T
 * A need to be symmetric positive definite
 * A [m*n]
 * L [m*n]
 * n == m
 */
void chol(const float *const A, float *L, uint16_t row)
{
	float s;
	uint16_t i, j, k;

	memset(L, 0, row * row * sizeof(float));
	for (i = 0; i < row; i++)
		for (j = 0; j <= i; j++) {
			s = 0;
			for (k = 0; k < j; k++)
				s += L[row * i + k] * L[row * j + k];

			// We cannot divide with zero
			if (L[row * j + j] == 0) {
				L[row * j + j] = FLT_EPSILON; // Same as eps command in MATLAB
			}
			L[row * i + j] = (i == j) ? sqrtf(A[row * i + i] - s) :
						    (1.0f / L[row * j + j] * (A[row * i + j] - s));
		}
}
