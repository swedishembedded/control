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
	float A[2][2] = {{5, 11}, {11, 25}};
	float L[2][2] = {{0, 0}, {0, 0}};
	float Lt[2][2] = {{0, 0}, {0, 0}};
	float Ar[2][2] = {{0, 0}, {0, 0}};

	// perform cholesky decomposition
	chol(A, L);

	// copy the L matrix and transpose the copy
	memcpy(&Lt[0][0], &L[0][0], sizeof(Lt));
	tran(Lt, Lt);

	// multiply L with L transposed to get A
	mul(Ar, L, Lt);

	// check result
	for (unsigned int i = 0; i < 2; i++) {
		for (unsigned int j = 0; j < 2; j++) {
			ASSERT_FLOAT_EQ(A[i][j], Ar[i][j]);
		}
	}
}
