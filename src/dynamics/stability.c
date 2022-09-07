// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"
#include "control/dynamics.h"

#include <math.h>

bool is_stable(const float *const A, uint8_t ADIM)
{
	float wr[ADIM]; // Real eigenvalues
	float wi[ADIM]; // Imaginary eigenvalues
	bool stable = true; // Assume that the system is stable

	eig(A, wr, wi, ADIM);
	for (uint8_t i = 0; i < ADIM; i++) {
		float abs_value = sqrtf(wr[i] * wr[i] + wi[i] * wi[i]);

		if (abs_value > 1) {
			stable = false;
		}
	}
	return stable;
}
