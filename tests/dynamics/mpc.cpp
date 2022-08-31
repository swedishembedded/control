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

TEST(Main, MPC)
{
#define ADIM 2
#define RDIM 1
#define YDIM 1
#define HORIZON 20
#define ITERATION_LIMIT 200

	// State space model that have integration behavior
	float A[ADIM * ADIM] = { 1.71653, 1.00000, -0.71653, 0.00000 };
	float B[ADIM * RDIM] = { 0.18699, 0.16734 };
	float C[YDIM * ADIM] = { 1, 0 };
	float x[ADIM] = { 0, 0 };
	float u[RDIM] = { 0 };
	float r[YDIM] = { 12.5 };
	float K[ADIM] = { 0, 0 };
	float y[YDIM] = { 0 };

	// Do Model Predictive Control where we selecting last u
	for (int i = 0; i < 200; i++) {
		mpc(A, B, C, x, u, r, ADIM, YDIM, RDIM, HORIZON, ITERATION_LIMIT, 1);
		kalman(x, A, x, B, u, K, y, C, ADIM, YDIM, RDIM);
		mul(y, C, x, YDIM, ADIM, ADIM, 1);
	}

	EXPECT_NEAR(r[0], y[0], 1e-3);

#undef ADIM
#undef RDIM
#undef YDIM
#undef HORIZON
#undef ITERATION_LIMIT
}
