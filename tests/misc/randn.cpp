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

TEST(Main, Randn)
{
	float x[10];
	randn(x, 10, 4, 2);
	EXPECT_NEAR(4, mean(x, 10), 1);
	EXPECT_NEAR(2, stddev(x, 10), 1);
}
