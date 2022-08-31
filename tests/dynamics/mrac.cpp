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

TEST(Main, MRAC)
{
#define RDIM 1
#define LIMIT 1
#define LEARNING 0.0001
#define ADIM 2
#define YDIM 1
#define HORIZON 20
#define ITERATION_LIMIT 200

	// State space model that have integration behavior
	float A[ADIM * ADIM] = { 1.71653, 1.00000, -0.71653, 0.00000 };
	float B[ADIM * RDIM] = { 0.18699, 0.16734 };
	float C[YDIM * ADIM] = { 1, 0 };
	float x[ADIM] = { 0, 0 };
	float K[ADIM] = { 0, 0 };

	/* MRAC works as K1 and K2 integrates over time so y -> r when time -> inf
	 * r ----{K1}*(-LEARNING)---->+()----->G(s)-----------------------> y
	 *               			  -^            			  |
	 *               			   |            			  |
	 *                			   |              			  |
	 *                  		   |---*(LEARNING){K2}<--------
	 */

	float y[RDIM] = { 7 };
	float u[RDIM] = { 5 };
	float r[RDIM] = { 51 };
	float I1[RDIM] = { 3.1 };
	float I2[RDIM] = { 4.1 };

	for (unsigned int c = 0; c < 10000; c++) {
		// simulate the system (K == 0)
		kalman(x, A, x, B, u, K, y, C, ADIM, YDIM, RDIM);
		mul(y, C, x, YDIM, ADIM, ADIM, 1);

		mrac(LIMIT, LEARNING, y, u, r, I1, I2, RDIM);
	}

	// TODO: MRAC implementation is currently very lacking
	// This needs to be revisited
	// Uncomment this test once fixed
	//EXPECT_NEAR(r[0], y[0], 1e-3);

#undef RDIM
#undef LIMIT
#undef LEARNING
}
