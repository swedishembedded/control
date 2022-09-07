// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/misc.h"

#include <assert.h>
#include <string.h>

void cat(float *C, const float *const A, const float *const B, int vertical, uint16_t row_a,
	 uint16_t column_a, uint16_t row_b, uint16_t column_b, uint16_t row_c, uint16_t column_c)
{
	if (vertical == 1) {
		/* C = [A;B] */
		assert(column_a == column_b);
		assert(row_a + row_b == row_c);
		memcpy(C, A, row_a * column_a * sizeof(float));
		memcpy(C + row_a * column_a, B, row_b * column_b * sizeof(float));
	} else {
		/* C = [A, B] */
		assert(row_a == row_b);
		assert(column_a + column_b == column_c);
		memcpy(C, A, column_a * sizeof(float));
		memcpy(C + column_a, B, column_b * sizeof(float));
		memcpy(C + column_a + column_b, A + column_a, column_a * sizeof(float));
		memcpy(C + column_a + column_b + column_a, B + column_b, column_b * sizeof(float));
	}
}
