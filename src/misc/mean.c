// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/misc.h"

float mean(const float *const x, uint16_t length)
{
	float s = 0;

	for (uint16_t i = 0; i < length; i++)
		s += x[i];

	return s / ((float)length);
}
