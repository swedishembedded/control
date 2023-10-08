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

TEST(Main, CholeskyUpdate)
{
	// A = pascal(3)
	float A[3][3] = { { 1, 1, 1 }, { 1, 2, 3 }, { 1, 3, 6 } };
	float L[9] = { 0 };
	float Lxxt[9] = { 0 };
	float x[3][1] = { { 0 }, { 0 }, { 1 } };
	float xt[1][3] = { { 0, 0, 1 } };
	float xxt[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
	float Axxt[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

	// perform cholesky decomposition
	// L = chol(A)
	chol(&A[0][0], L, 3);
	// perform update using second cholesky
	// Lxxt = chol(A + x * x')
	tran(xt, x);
	mul(xxt, x, xt);
	add(Axxt, A, xxt);
	chol(&Axxt[0][0], Lxxt, 3);

	// perform the same operation using cholupdate
	// Lxxt = cholupdate(L, x)
	cholupdate(L, &x[0][0], 3, 1);

	// Lxxt and Axxt should be the same
	for (unsigned int c = 0; c < 4; c++) {
		ASSERT_FLOAT_EQ(L[c], Lxxt[c]);
	}
}
