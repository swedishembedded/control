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

void okid(float u[], float y[], float g[], uint16_t row, uint16_t column)
{
	/**
	 * This is just a simple linear solve Ax = b where A is lower toeplitz
	 * triangular shape * but A is a vector of g. So the formula is y' = g*u
	 * and we want to solve g = y/u
	 * u is a vector, but it's interpreted as
	 * [u0  0  0  0  0 0 0]  [g0] [y0]
	 * [u1 u0 0 0 0 0 0]  [g1] [y1]
	 * [u2 u1 u0 0 0 0 0]  [g2] [y2]
	 * [u3 u2 u1 u0 0 0 0]   *  [g3] =  [y3]
	 * [u4 u3 u2 u1 u0 0 0]  [g4] [y4]
	 * [u5 u4 u3 u2 u1 u0 0]  [g5] [y5]
	 * [.. u5 u4 u3 u2 u1 u0]  [g6] [y6]
	 * [un .. .. .. .. .. ..]  [gn] [yn]
	 *
	 * Where g0 = y0/u0 and g1 = (y1 - u1*g0)/u0 etc..
	 */
	memset(g, 0, row * column * sizeof(float));
	float sum;

	// If we have more than 1 rows = MIMO system
	for (uint16_t k = 0; k < row; k++) {
		for (uint16_t i = 0; i < column; i++) {
			sum = 0;
			for (int j = 0; j < i; j = j + 1) {
				sum = sum + u[k * column + i - j] * g[k * column + j];
			}
			g[k * column + i] = (y[k * column + i] - sum) / u[k * column + 0];
		}
	}
}
