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

/*
 * GNU Octave code:
A = [4 23 5; 2 45 5]
	A =

		4   23    5
		2   45    5

	>> A'
	ans =

4    2
23   45
5    5
 */

TEST(Main, Tran)
{
	// cmake-format off
	float A[2 * 2] = { 1, 2, 3, 4 };
	float At[2 * 2];
	float At_exp[2 * 2] = { 1, 3, 2, 4 };
	// cmake-format on

	tran(At, A, 2, 2);

	for (unsigned int c = 0; c < 2 * 2; c++) {
		ASSERT_FLOAT_EQ(At_exp[c], At[c]);
	}
}
