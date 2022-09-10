// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/sysid.h"

#include <string.h>

void linsolve_markov(float *g, const float *const y, const float *const u, uint16_t row,
		     uint16_t column)
{
	memset(g, 0, row * column * sizeof(float));

	// If we have more than 1 rows = MIMO system
	for (uint16_t k = 0; k < row; k++) {
		for (uint16_t i = 0; i < column; i++) {
			float sum = 0;
			for (int j = 0; j < i; j++) {
				sum += u[k * column + i - j] * g[k * column + j];
			}
			g[k * column + i] = (y[k * column + i] - sum) / u[k * column + 0];
		}
	}
}
