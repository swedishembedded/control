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

void sum(float *Ar, const float *const A, uint16_t row, uint16_t column, uint8_t l)
{
	memcpy(Ar, A, sizeof(float) * row * column);
	if (l == 1) {
		for (uint16_t i = 1; i < row; i++) {
			for (uint16_t j = 0; j < column; j++) {
				Ar[j] += A[i * column + j];
			}
		}
	} else if (l == 2) {
		for (uint16_t i = 0; i < row; i++) {
			for (uint16_t j = 1; j < column; j++) {
				Ar[i * column] += A[i * column + j];
			}
		}
	}
}
