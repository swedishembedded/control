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

TEST(Main, Sum)
{
	// clang-format off
	float A[3 * 3] = {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9
	};
	float Ar1[3 * 3];
	float Ar2[3 * 3];
	float Ar1_exp[3 * 3] = {
		12, 15, 18,
		4, 5, 6,
		7, 8, 9,
	};
	float Ar2_exp[3 * 3] = {
		6, 2, 3,
		15, 5, 6,
		24, 8, 9,
	};
	// clang-format on

	sum(Ar1, A, 3, 3, 1);
	sum(Ar2, A, 3, 3, 2);

	for (unsigned int c = 0; c < 3 * 3; c++) {
		ASSERT_FLOAT_EQ(Ar1_exp[c], Ar1[c]);
	}
	for (unsigned int c = 0; c < 3 * 3; c++) {
		ASSERT_FLOAT_EQ(Ar2_exp[c], Ar2[c]);
	}
}
