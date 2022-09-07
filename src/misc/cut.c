// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/misc.h"

#include <string.h>

void cut(float *B, const float *const A, uint16_t row, uint16_t column, uint16_t start_row,
	 uint16_t start_column, uint16_t row_b, uint16_t column_b)
{
	(void)row;
	uint16_t stop_row = start_row + row_b;
	uint16_t stop_column = start_column + column_b;
	int in_columns = column;
	const float *data = A + start_row * in_columns + start_column;

	// Create the output
	int out_columns = stop_column - start_column;

	// Instead of having two for loops, we just copy the whole row at once.
	for (int i = start_row; i < stop_row; i++) {
		memcpy(B, data, sizeof(float) * out_columns);
		B += out_columns;
		data += in_columns;
	}
}
