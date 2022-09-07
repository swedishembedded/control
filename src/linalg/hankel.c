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
#include <string.h>

/*
 * Create hankel matrix of vector V. Step is just the shift. Normaly set this to 0.
 * V [m*n]
 * H [m*n]
 * shift >= 0 // Set this to 0 if you want a normal hankel matrix
 */
int hankel(const float *const V, float *H, uint16_t row_v, uint16_t column_v, uint16_t row_h,
	   uint16_t column_h, uint16_t shift)
{
	// row_h need to be divided with row_v
	if (row_h % row_v != 0)
		// Cannot create hankel matrix
		return -EINVAL;

	memset(H, 0, row_h * column_h * sizeof(float));

	uint16_t delta = 0;

	for (uint16_t i = 0; i < row_h / row_v; i++) {
		for (uint16_t j = 0; j < row_v; j++) {
			// Compute how much we should load H for every row
			if (column_v > column_h + i)
				delta = column_v - column_h;
			else
				delta = column_h - i;

			// We cannot overindexing the V array. Lower the delta
			if ((row_v - 1) * column_v + i + shift + delta > row_v * column_v) {
				delta--;
			}

			memcpy(H + (i * row_v + j) * column_h, V + j * column_v + i + shift,
			       delta * sizeof(float));
		}
	}

	return 0;
}
