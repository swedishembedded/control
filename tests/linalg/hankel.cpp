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

TEST(Main, HankelMatrix)
{
	// clang-format off
	float V[2 * 10] = {
		3, 51, 3, 2, 6, 7, 8, 9, 1, 4,
		1, 3, 1, 4, 5, 1, 7, 8, 1, 10
	};
	float A[1 * 10] = { 0, 2, 4, 1, 4, 5, 7, 2, 7, 8 };

	float H0_exp[10 * 5] = {
		51, 3, 2, 6, 7,
		3, 1, 4, 5, 1,
		3, 2, 6, 7, 8,
		1, 4, 5, 1, 7,
		2, 6, 7, 8, 9,
		4, 5, 1, 7, 8,
		6, 7, 8, 9, 1,
		5, 1, 7, 8, 1,
		7, 8, 9, 1, 4,
		1, 7, 8, 1, 10,
	};

	float H1_exp[20 * 10] = {
		3, 51, 3, 2, 6, 7, 8, 9, 1, 4,
		1, 3, 1, 4, 5, 1, 7, 8, 1, 10,
		51, 3, 2, 6, 7, 8, 9, 1, 4, 0,
		3, 1, 4, 5, 1, 7, 8, 1, 10, 0,
		3, 2, 6, 7, 8, 9, 1, 4, 0, 0,
		1, 4, 5, 1, 7, 8, 1, 10, 0, 0,
		2, 6, 7, 8, 9, 1, 4, 0, 0, 0,
		4, 5, 1, 7, 8, 1, 10, 0, 0, 0,
		6, 7, 8, 9, 1, 4, 0, 0, 0, 0,
		5, 1, 7, 8, 1, 10, 0, 0, 0, 0,
		7, 8, 9, 1, 4, 0, 0, 0, 0, 0,
		1, 7, 8, 1, 10, 0, 0, 0, 0, 0,
		8, 9, 1, 4, 0, 0, 0, 0, 0, 0,
		7, 8, 1, 10, 0, 0, 0, 0, 0, 0,
		9, 1, 4, 0, 0, 0, 0, 0, 0, 0,
		8, 1, 10, 0, 0, 0, 0, 0, 0, 0,
		1, 4, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 10, 0, 0, 0, 0, 0, 0, 0, 0,
		4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		10, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	float H2_exp[10 * 10] = {
		0, 2, 4, 1, 4, 5, 7, 2, 7, 8,
		2, 4, 1, 4, 5, 7, 2, 7, 8, 0,
		4, 1, 4, 5, 7, 2, 7, 8, 0, 0,
		1, 4, 5, 7, 2, 7, 8, 0, 0, 0,
		4, 5, 7, 2, 7, 8, 0, 0, 0, 0,
		5, 7, 2, 7, 8, 0, 0, 0, 0, 0,
		7, 2, 7, 8, 0, 0, 0, 0, 0, 0,
		2, 7, 8, 0, 0, 0, 0, 0, 0, 0,
		7, 8, 0, 0, 0, 0, 0, 0, 0, 0,
		8, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	// clang-format on

	// Hankel matrix - Remember! If we want V into H and V is more then 1 row
	// then we need to increase the rows of hanekel matrix H
	// Half hankel matrix - Yes, that's possible too! Used in era() function
	float H0[(2 * 5) * 5];
	// Normal hankel matrix
	float H1[(2 * 10) * 10];
	// Normal hankel matrix
	float H2[10 * 10];

	hankel(V, H0, 2, 10, 2 * 5, 5, 1); // Shift = 1
	hankel(V, H1, 2, 10, 2 * 10, 10, 0); // Shift = 0
	hankel(A, H2, 1, 10, 10, 10, 0);

	for (unsigned c = 0; c < sizeof(H0) / sizeof(float); c++) {
		ASSERT_NEAR(H0_exp[c], H0[c], 1e-5);
	}

	for (unsigned c = 0; c < sizeof(H1) / sizeof(float); c++) {
		ASSERT_NEAR(H1_exp[c], H1[c], 1e-5);
	}

	for (unsigned c = 0; c < sizeof(H2) / sizeof(float); c++) {
		ASSERT_NEAR(H2_exp[c], H2[c], 1e-5);
	}
}
