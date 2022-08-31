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
#include "control/misc.h"
};

TEST(Main, Cat)
{
	// clang-format off
	float A[2 * 3] = {
		1, 2, 3,
		4, 5, 6
	};
	float B[2 * 4] = {
		7, 8, 9, 6,
		1, 2, 3, 5
	};
	float C[2 * 7] = { 0 };
	float C_exp[2 * 7] = {
		1, 2, 3, 7, 8, 9, 6,
		4, 5, 6, 1, 2, 3, 5
	};
	// clang-format on

	cat(C, A, B, 0, 2, 3, 2, 4, 2, 7); /* do C = [A B] */

	for (unsigned int c = 0; c < 2 * 7; c++) {
		ASSERT_FLOAT_EQ(C_exp[c], C[c]);
	}
}
