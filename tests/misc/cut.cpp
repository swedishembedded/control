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
#include "control/dynamics.h"
#include "control/linalg.h"
#include "control/misc.h"
};

TEST(Main, Cut)
{
	// clang-format off
	float A[5 * 5] = {
		0.87508, 0.48607, 0.30560, 0.32509, 0.23096,
		0.12308, 0.84311, 0.42221, 0.20273, 0.87377,
		0.42986, 0.11245, 0.40494, 0.27304, 0.59772,
		0.85124, 0.99245, 0.56873, 0.19438, 0.11308,
		0.97190, 0.22475, 0.24501, 0.85403, 0.54691
	};
	// clang-format on

	// Matrix B
	float B[2 * 3];

	// Cut a matrix
	// We want to B = A(4:5, 3:5)
	cut(B, A, 5, 5, 3, 2, 2, 3);

	printf("B\n");
	print(B, 2, 3);

	for (unsigned int i = 3; i < 5; i++) {
		for (unsigned int j = 2; j < 5; j++) {
			EXPECT_FLOAT_EQ(A[i * 5 + j], B[(i - 3) * 3 + (j - 2)]);
		}
	}
}
