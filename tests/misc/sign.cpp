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

TEST(Main, Sign)
{
	EXPECT_FLOAT_EQ(-1, sign(-10));
	EXPECT_FLOAT_EQ(1, sign(123));
	EXPECT_FLOAT_EQ(0, sign(0));
}
