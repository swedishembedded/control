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

TEST(Main, Print)
{
	// NOTE: this test is mostly just a dummy
	// Print is mostly used for testing and debugging anyway
	float A[4] = { 1, -4, 2, 4 };
	print(A, 2, 2);
}
