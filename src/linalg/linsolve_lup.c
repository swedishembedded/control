// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include <errno.h>

#include <control/linalg.h>

int linsolve_lup(const float *const A, float *x, const float *const b, uint16_t row)
{
	float LU[row * row];
	uint8_t P[row];

	if (lup(A, LU, P, row) != 0) {
		return -ENOTSUP;
	}

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

		x[i] = x[i] / LU[row * P[i] + i];

		if (i == 0) {
			break;
		}
	}

	return 0;
}
