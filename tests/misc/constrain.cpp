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

TEST(Main, Constrain)
{
	float x[3] = { 4, 2, -1 };
	float x_out[3] = { 0 };
	EXPECT_FLOAT_EQ(4, constrain_float(8, 0, 4));
	EXPECT_FLOAT_EQ(0, constrain_float(-1, 0, 4));
	EXPECT_FLOAT_EQ(1, constrain_float(1, 0, 4));
	constrain(x_out, x, 3, 1, 2);
	EXPECT_FLOAT_EQ(x_out[0], 2);
	EXPECT_FLOAT_EQ(x_out[2], 1);
}
