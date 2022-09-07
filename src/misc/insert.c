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

void insert(float *B, const float *const A, uint16_t row_a, uint16_t column_a, uint16_t column_b,
	    uint16_t startRow_b, uint16_t startColumn_b)
{
	const float *Apos = A;

	// Now we are standing on position startRow_b x startColumn_b of row_b x column_b
	B += startRow_b * column_b + startColumn_b;

	// We start at startRow_b and end at row_a, then we have inserted all rows from A into B
	for (uint16_t i = 0; i < row_a; i++) {
		memcpy(B, Apos, column_a * sizeof(float)); // Insert the complete row from A into B
		Apos += column_a;
		B += column_b;
	}
}
