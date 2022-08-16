// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include <control/misc.h>

/*
 * This will saturate the input value with limits
 */
float saturation(float input, float lower_limit, float upper_limit)
{
	if (input > upper_limit) {
		return upper_limit;
	} else if (input < lower_limit) {
		return lower_limit;
	} else {
		return input; // No action
	}
}
