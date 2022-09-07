/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/dynamics.h"
};

TEST(Main, C2D)
{
	// clang-format off
	float A_exp[] = {51.968956,  74.736565, 112.10485,  164.07379};
	float B_exp[] = {13.600674, 30.567947};
	float A[] = { 1, 2, 3, 4 };
	float B[] = {0, 1};
	float Ad[4];
	float Bd[2];

	// clang-format on

	c2d(Ad, Bd, A, B, 2, 1, 1);

	for (unsigned int c = 0; c < 4; c++) {
		ASSERT_FLOAT_EQ(A_exp[c], Ad[c]);
	}

	for (unsigned int c = 0; c < 2; c++) {
		ASSERT_FLOAT_EQ(B_exp[c], Bd[c]);
	}
}
