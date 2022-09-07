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

int mul(float *C, const float *const A, const float *const B, uint16_t row_a, uint16_t column_a,
	uint16_t row_b, uint16_t column_b)
{
	// Data matrix
	const float *data_a;
	const float *data_b;

	if (column_a != row_b) {
		return -EINVAL;
	}

	for (uint16_t i = 0; i < row_a; i++) {
		// Then we go through every column of b
		for (uint16_t j = 0; j < column_b; j++) {
			data_a = &A[i * column_a];
			data_b = &B[j];

			*C = 0; // Reset
			// And we multiply rows from a with columns of b
			for (uint16_t k = 0; k < column_a; k++) {
				*C += *data_a * *data_b;
				data_a++;
				data_b += column_b;
			}
			C++;
		}
	}
	return 0;
}

/*
 * GNU Octave code:
 *  >> A = [4 23; 2  5];
	>> B = [3; 1];
	>> C = A*B
	C =

	   35
	   11

	>>
 *
 */
