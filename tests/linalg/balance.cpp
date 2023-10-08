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

TEST(Main, BalanceSquareMatrix)
{
	float A[2][2] = {{-0.1, 10.0}, {-1.0, -5.0}};
	float Abal[2][2] = {{-0.1, 2.5}, {-4.0, -5.0}};

	balance(A);

	for (unsigned int i = 0; i < 2; i++) {
		for (unsigned int j = 0; j < 2; j++) {
			ASSERT_FLOAT_EQ(Abal[i][j], A[i][j]);
		}
	}
}
