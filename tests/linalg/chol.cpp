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

TEST(Main, CholeskyDecomposition)
{
	float A[] = { 5, 11, 11, 25 };
	float L[4] = { 0 };
	float Lt[4] = { 0 };
	float Ar[4] = { 0 };

	// perform cholesky decomposition
	chol(A, L, 2);
	// copy the L matrix and transpose the copy
	memcpy(Lt, L, sizeof(Lt));
	tran(Lt, Lt, 2, 2);
	// multiply L with L transposed to get A
	mul(Ar, L, Lt, 2, 2, 2, 2);
	// check result
	for (unsigned int c = 0; c < 4; c++) {
		ASSERT_FLOAT_EQ(A[c], Ar[c]);
	}
}
