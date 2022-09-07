// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"

void linsolve_chol(const float *const A, float *x, const float *const b, uint16_t row)
{
	float L[row * row];
	float y[row];

	chol(A, L, row);
	linsolve_lower_triangular(L, y, b, row);
	tran(L, L, row, row);
	linsolve_upper_triangular(L, x, y, row);
}
