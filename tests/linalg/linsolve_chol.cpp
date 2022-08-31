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

TEST(Main, LinsolveChol)
{
	// Matrix A
	// clang-format off
	float A[5 * 5] = {
		1.1000,  2.0000,  3.0000,  4.0000,  5.0000,
		2.0000,  4.1000,  6.0000,  8.0000, 10.0000,
		3.0000,  6.0000,	9.1000, 12.0000, 15.0000,
		4.0000,  8.0000, 12.0000, 16.1000, 20.0000,
		5.0000, 10.0000, 15.0000, 20.0000, 25.1000 };
	// clang-format on

	// Vector b
	float b[5] = { 5, 3, 2, 5, 7 };

	// Solution that need to have the same rows and columns from A
	float x[5];

	float x_exp[5] = { 36.9328, 3.8657, -19.2015, -2.2686, 4.6642 };

	// Do Cholesky decomposition and then solve with linsolve_chol
	linsolve_chol(A, x, b, 5);

	for (unsigned int c = 0; c < 5; c++) {
		EXPECT_NEAR(x_exp[c], x[c], 1e-3);
	}
}
