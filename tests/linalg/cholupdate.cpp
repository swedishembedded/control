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
	float A[3][3] = {{1, 1, 1}, {1, 2, 3}, {1, 3, 6}};
	float L[3][3] = {{0}};
	float Lxxt[3][3] = {{0}};
	float x[3][1] = {{0}, {0}, {1}};
	float xt[1][3] = {{0, 0, 1}};
	float xxt[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	float Axxt[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

	// perform cholesky decomposition
	// L = chol(A)
	chol(A, L);
	// perform update using second cholesky
	// Lxxt = chol(A + x * x')
	tran(xt, x);
	mul(xxt, x, xt);
	add(Axxt, A, xxt);
	chol(Axxt, Lxxt);

	// perform the same operation using cholupdate
	// Lxxt = cholupdate(L, x)
	cholupdate(&L[0][0], &x[0][0], 3, 1);

	// Lxxt and Axxt should be the same
	for (unsigned int i = 0; i < 2; i++) {
		for (unsigned int j = 0; j < 2; j++) {
			ASSERT_FLOAT_EQ(L[i][j], Lxxt[i][j]);
		}
	}
}
