// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/misc.h"

float constrain_float(float input, float lower_limit, float upper_limit)
{
	if (input > upper_limit) {
		return upper_limit;
	}

	if (input < lower_limit) {
		return lower_limit;
	}
	// unchanged
	return input;
}

void constrain(float *I, const float *const Iin, uint16_t dim, float lower, float upper)
{
	for (unsigned int i = 0; i < dim; i++) {
		I[i] = constrain_float(Iin[i], lower, upper);
	}
}
