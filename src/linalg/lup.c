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

int lup(const float *const A, float *LU, uint8_t *P, uint16_t row)
{
	// If not the same
	if (A != LU)
		memcpy(LU, A, row * row * sizeof(float));

	// Create the pivot vector
	for (uint16_t i = 0; i < row; ++i) {
		P[i] = i;
	}

	for (uint16_t i = 0; i < row - 1; ++i) {
		uint16_t ind_max = i;

		for (uint16_t j = i + 1; j < row; ++j)
			if (fabsf(LU[row * P[j] + i]) > fabsf(LU[row * P[ind_max] + i]))
				ind_max = j;

		uint16_t tmp_int = P[i];

		P[i] = P[ind_max];
		P[ind_max] = tmp_int;

		if (fabsf(LU[row * P[i] + i]) < FLT_EPSILON)
			return -ENOTSUP; // matrix is singular (up to tolerance)

		for (uint16_t j = i + 1; j < row; ++j) {
			LU[row * P[j] + i] = LU[row * P[j] + i] / LU[row * P[i] + i];

			for (uint16_t k = i + 1; k < row; ++k)
				LU[row * P[j] + k] = LU[row * P[j] + k] -
						     LU[row * P[i] + k] * LU[row * P[j] + i];
		}
	}

	return 0;
}
