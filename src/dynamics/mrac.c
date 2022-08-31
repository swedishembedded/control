// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"
#include "control/misc.h"
#include "control/dynamics.h"

static void integral(float I[], float gain, float x[], float e[], uint8_t RDIM);
static void modelerror(float e[], float y[], float r[], uint8_t RDIM);
static void findinput(float u[], float r[], float I1[], float y[], float I2[], uint8_t RDIM);

void mrac(float limit, float gain, float y[], float u[], float r[], float I1[], float I2[],
	  uint8_t RDIM)
{
	// Find the model error
	float e[RDIM];

	modelerror(e, y, r, RDIM);

	// Integrate
	integral(I1, -gain, r, e, RDIM); // I1 = I1 - gain*r*e
	integral(I2, gain, y, e, RDIM); // I2 = I2 + gain*y*e

	// Saturate
	constrain(I1, I1, RDIM, -limit, limit);
	constrain(I2, I2, RDIM, -limit, limit);

	// Find input signal
	findinput(u, r, I1, y, I2, RDIM);
}

static void integral(float I[], float gain, float x[], float e[], uint8_t RDIM)
{
	for (uint8_t i = 0; i < RDIM; i++)
		I[i] += gain * x[i] * e[i];
}

static void modelerror(float e[], float y[], float r[], uint8_t RDIM)
{
	for (uint8_t i = 0; i < RDIM; i++)
		e[i] = y[i] - r[i];
}

static void findinput(float u[], float r[], float I1[], float y[], float I2[], uint8_t RDIM)
{
	for (uint8_t i = 0; i < RDIM; i++)
		u[i] = r[i] * I1[i] - y[i] * I2[i];
}
