// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include <string.h>
#include <control/linalg.h>

void tran(float *At, const float *const A, uint16_t row, uint16_t column)
{
	float B[row * column];
	float *transpose;
	const float *ptr_A = A;

	for (uint16_t i = 0; i < row; i++) {
		transpose = &B[i];
		for (uint16_t j = 0; j < column; j++) {
			*transpose = *ptr_A;
			ptr_A++;
			transpose += row;
		}
	}

	// Copy!
	memcpy(At, B, row * column * sizeof(float));
}
