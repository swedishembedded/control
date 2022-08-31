// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include <control/misc.h>

float sign(float number)
{
	float s;

	if (number > 0)
		s = 1.0f;
	else if (number < 0)
		s = -1.0f;
	else
		s = 0.0f;
	return s;
}
