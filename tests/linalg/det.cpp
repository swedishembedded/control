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

TEST(Main, Determinant)
{
	float A0[] = { 1, 2, 3, 4 };
	float A1[] = { 23, -1, -40, 50 };
	float A2[] = { 23 };

	ASSERT_FLOAT_EQ(-2, det(A0, 2));
	ASSERT_FLOAT_EQ(1110, det(A1, 2));
	ASSERT_FLOAT_EQ(23, det(A2, 1));
}
