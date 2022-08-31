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

TEST(Main, DiscreteLyapunov1)
{
	float A[2 * 2] = { 0.798231, 0.191700, -0.575101, 0.031430 };
	float Q[2 * 2] = { 1, 2, 2, 3 };
	float P[2 * 2];
	float Pex[2 * 2] = { 3.50103, 0.38501, 0.38501, 4.14811 };

	dlyap(A, P, Q, 2);

	for (unsigned c = 0; c < 4; c++) {
		ASSERT_NEAR(Pex[c], P[c], 1e-5);
	}
}

TEST(Main, DiscreteLyapunov2)
{
	float A[2 * 2] = { 11, 21, -5, -1 };
	float Q[2 * 2] = { 1, 0, 0, 2 };
	float P[2 * 2];
	float Pex[2 * 2] = { -0.101202, 0.053004, 0.053004, -0.030258 };

	dlyap(A, P, Q, 2);

	for (unsigned c = 0; c < 4; c++) {
		ASSERT_NEAR(Pex[c], P[c], 1e-5);
	}
}
