// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"

#include <string.h>

/*
 * C = A + B
 */
void add(float *C, const float *const A, const float *const B, uint16_t row, uint16_t column)
{
	float Cr[row * column];

	for (uint16_t i = 0; i < row; i++) {
		for (uint16_t j = 0; j < column; j++) {
			Cr[i * column + j] = A[i * column + j] + B[i * column + j];
		}
	}
	memcpy(C, Cr, sizeof(float) * row * column);
}
