// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/misc.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

static float generate_gauss(float mu, float sigma)
{
	float U1, U2, W, scalar;
	static float X1, X2;
	static int call;

	if (call == 1) {
		call = !call;
		return (mu + sigma * X2);
	}

	// Compute the uniform norm
	do {
		U1 = -1.0f + ((float)rand() / (float)RAND_MAX) * 2.0f;
		U2 = -1.0f + ((float)rand() / (float)RAND_MAX) * 2.0f;
		W = powf(U1, 2) + powf(U2, 2);
	} while (W >= 1.0f || W == 0.0f);

	// TODO: potential division by zero
	scalar = sqrtf((-2.0f * logf(W)) / W);
	X1 = U1 * scalar;
	X2 = U2 * scalar;

	call = !call;

	return (mu + sigma * X1);
}

void randn(float *x, uint16_t length, float mu, float sigma)
{
	for (uint16_t i = 0; i < length; i++)
		x[i] = generate_gauss(mu, sigma);
}
