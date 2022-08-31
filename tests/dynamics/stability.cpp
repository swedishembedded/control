/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <math.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/dynamics.h"
#include "control/linalg.h"
};

TEST(Main, IsStable)
{
	// matrix with second eigenvalue > 1
	// eig(A) = -0.37228, 5.37228
	float A[2 * 2] = { 1, 2, 3, 4 };
	EXPECT_FALSE(is_stable(A, 2));
	float A2[2 * 2] = { 1, 0, -1, -1 };
	EXPECT_TRUE(is_stable(A2, 2));
}
