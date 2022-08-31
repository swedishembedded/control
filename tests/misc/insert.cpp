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

TEST(Main, Insert)
{
	// clang-format off
	float B[5 * 5] = {
		0.87508, 0.48607, 0.30560, 0.32509, 0.23096,
		0.12308, 0.84311, 0.42221, 0.20273, 0.87377,
		0.42986, 0.11245, 0.40494, 0.27304, 0.59772,
		0.85124, 0.99245, 0.56873, 0.19438, 0.11308,
		0.97190, 0.22475, 0.24501, 0.85403, 0.54691
	};
	float A[2 * 3] = {
		1, 2, 3,
		4, 5, 6
	};
	// clang-format on

	// Paste A in position 2 2 in B
	insert(B, A, 2, 3, 5, 2, 2);

	printf("B\n");
	print(B, 5, 5);

	for (unsigned int i = 0; i < 2; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			EXPECT_FLOAT_EQ(A[i * 3 + j], B[(2 + i) * 5 + (2 + j)]);
		}
	}
}
