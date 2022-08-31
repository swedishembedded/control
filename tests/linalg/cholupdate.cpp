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
	float A[] = { 1, 1, 1, 1, 2, 3, 1, 3, 6 };
	float L[9] = { 0 };
	float Lxxt[9] = { 0 };
	float x[3] = { 0, 0, 1 };
	float xt[3] = { 0, 0, 1 };
	float xxt[9] = { 0 };
	float Axxt[9] = { 0 };

	// perform cholesky decomposition
	// L = chol(A)
	chol(A, L, 3);
	// perform update using second cholesky
	// Lxxt = chol(A + x * x')
	tran(xt, xt, 3, 1);
	mul(xxt, x, xt, 3, 1, 1, 3);
	add(Axxt, A, xxt, 3, 3);
	chol(Axxt, Lxxt, 3);

	// perform the same operation using cholupdate
	// Lxxt = cholupdate(L, x)
	cholupdate(L, x, 3, 1);

	// Lxxt and Axxt should be the same
	for (unsigned int c = 0; c < 4; c++) {
		ASSERT_FLOAT_EQ(L[c], Lxxt[c]);
	}
}
