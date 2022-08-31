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
#include "control/linalg.h"
};

TEST(Main, LinsolveLup)
{
	// Matrix A
	// clang-format off
	float A[4 * 4] = {
		0.47462, 0.74679, 0.31008, 0.63073,
		0.32540, 0.49584, 0.50932, 0.21492,
		0.43855, 0.98844, 0.54041, 0.24647,
		0.62808, 0.72591, 0.20244, 0.96743 };

	// Vector b
	float b[4] = { 1.588964, 0.901248, 0.062029, 0.142180 };
	// clang-format on

	// Solution that need to have the same rows and columns from A
	float x[4];

	float x_exp[4] = { -44.1551, 6.1363, 15.1259, 21.0440 };

	// Do LUP-decomposition and then solve with linsolve_lup
	ASSERT_EQ(0, linsolve_lup(A, x, b, 4));

	for (unsigned int c = 0; c < 4; c++) {
		EXPECT_NEAR(x_exp[c], x[c], 1e-3);
	}
}
