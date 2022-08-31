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

TEST(Main, Stddev)
{
	// clang-format off
	float x[10] = {
		3, 11, -8, 1, 4,
		8, 9,   2, 5, 3
	};
	// clang-format on
	EXPECT_NEAR(3.8000, mean(x, 10), 1e-2);
	EXPECT_NEAR(5.2662, stddev(x, 10), 1e-2);
}
