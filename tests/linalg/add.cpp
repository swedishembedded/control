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

TEST(Main, MatrixAddition)
{
	float A[] = { 1, -2, 4, 5 };
	float B[] = { -2, -8, 2, 1 };
	float A_exp[] = { -1, -10, 6, 6 };

	add(A, A, B, 2, 2);

	for (unsigned int c = 0; c < 4; c++) {
		ASSERT_FLOAT_EQ(A[c], A_exp[c]);
	}
}
