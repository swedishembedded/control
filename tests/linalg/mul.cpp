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

TEST(Main, Mul)
{
	float A[2 * 2] = { 1, 4, 2, 3 };
	float x[2] = { 2, 1 };
	float C[2];
	float C_exp[2] = { 6, 7 };

	mul(C, A, x, 2, 2, 2, 1);

	for (unsigned int c = 0; c < 2; c++) {
		EXPECT_NEAR(C_exp[c], C[c], 1e-3);
	}
}

TEST(Main, MulDifferentSize)
{
	float A[2 * 3] = { 1, 2, 3, 4, 5, 6 };
	float x[3 * 2] = { 2, 1, 3, 5, 3, 7 };
	float C[2 * 2];
	float C_exp[2 * 2] = { 17, 32, 41, 71 };

	mul(C, A, x, 2, 3, 3, 2);

	for (unsigned int c = 0; c < 2 * 2; c++) {
		EXPECT_NEAR(C_exp[c], C[c], 1e-3);
	}
}
