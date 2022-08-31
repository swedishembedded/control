/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Copyright 2017 Daniel Mårtensson
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/dynamics.h"
#include "control/linalg.h"
};

TEST(Main, LQI)
{
#define ADIM 2
#define RDIM 1
#define YDIM 1
#define ANTI_WINDUP 0

	// Model A, B, C, D
	float A[ADIM * ADIM] = { 0.89559, 0.37735, -0.37735, 0.51825 };

	float B[ADIM * RDIM] = { 0.20881, 0.75469 };

	float C[YDIM * ADIM] = { 1, 0 };

	// Kalman steady state matrix K
	float K[ADIM * YDIM] = { 0.58006, -0.22391 };

	// Control law L
	float L[RDIM * ADIM] = { 1.56766, 0.85103 };

	// Integral law Li
	float Li[RDIM] = { 0.50135 };

	// Initial states - Normally set to zero
	float x[ADIM] = { 0 };
	float xerr[ADIM] = { 0 };
	float xi[1] = { 0 };

	// Input signal, reference signal, output signal and state vector x and xi
	float qi = 0.1; // <-- Tune this in between 0->1, but qi =/= 1.
	float u[RDIM] = { 0 }; // <-- This is our input value we want to have
	float r[RDIM] = { 25 }; // <-- This is what we want to see.
	float y[YDIM] = { 0 }; // <-- This is our output error estimate
	float err[YDIM] = { 0 };

	// iterate
	for (unsigned int c = 0; c < 200; c++) {
		// r - y is error but since our "controller" reference is always 0 we negate it
		err[0] = -(r[0] - y[0]);

		// error state estimation (controller side)
		{
			// Generate control action to drive error state to zero
			lqi(u, L, xerr, Li, xi, err, ADIM, YDIM, RDIM, ANTI_WINDUP);
			// Estimate the error state xerr
			kalman(xerr, A, xerr, B, u, K, err, C, ADIM, YDIM, RDIM);
		}

		// plant state simulation (typically physical plant)
		{
			// Estimate our plant state (fake plant)
			kalman(x, A, x, B, u, K, y, C, ADIM, YDIM, RDIM);

			// Fake measurement. You can add noise here
			mul(y, C, x, YDIM, ADIM, ADIM, 1);
		}
	}
	ASSERT_NEAR(r[0], y[0], 1e-3);

#undef RDIM
#undef YDIM
#undef ADIM
}
