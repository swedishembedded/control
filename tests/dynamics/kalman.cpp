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
};

TEST(Main, Kalman)
{
#define ADIM 2
#define RDIM 2
#define YDIM 2

	// clang-format off
	float A[ADIM * ADIM] = { 0.79499, 0.33654, -0.70673, 0.35749 };
	float B[ADIM * RDIM] = { 0.2050072, 0.0097622, 0.7067279, 0.0336537 };
	float C[YDIM * ADIM] = { 1.20000, 0.00000, 0.00000, 0.30000 };
	float K[ADIM * YDIM] = { 0.532591, -0.066108, -0.503803, 0.136580 };
	// clang-format on

	// Create input u, output y and state vector x
	float u[RDIM] = { 2.2, 4.3 };
	float y[YDIM] = { 5.134, 0.131 };
	float x[ADIM] = { 2.32, 4.12 };

	// Call kalman function and estimate our future state at x(k+1)
	kalman(x, A, x, B, u, K, y, C, ADIM, YDIM, RDIM);
#undef RDIM
#undef YDIM
#undef ADIM

	float x_exp[] = { 5.04855, 0.19790 };

	for (unsigned int c = 0; c < 2; c++) {
		ASSERT_NEAR(x_exp[c], x[c], 1e-3);
	}
}
