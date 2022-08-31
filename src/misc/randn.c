// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "control/misc.h"

static float generate_gauss(float mu, float sigma)
{
	float U1, U2, W, scalar;
	static float X1, X2;
	static int call;

	if (call == 1) {
		call = !call;
		return (mu + sigma * (float)X2);
	}

	// Compute the uniform norm
	do {
		U1 = -1 + ((float)rand() / RAND_MAX) * 2;
		U2 = -1 + ((float)rand() / RAND_MAX) * 2;
		W = pow(U1, 2) + pow(U2, 2);
	} while (W >= 1 || W == 0);

	scalar = sqrt((-2 * log(W)) / W);
	X1 = U1 * scalar;
	X2 = U2 * scalar;

	call = !call;

	return (mu + sigma * (float)X1);
}

void randn(float *x, uint16_t length, float mu, float sigma)
{
	for (uint16_t i = 0; i < length; i++)
		x[i] = generate_gauss(mu, sigma);
}
