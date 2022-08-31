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
#include "control/misc.h"
};

/*
 * GNU Octave code:
A = [0.453690,   0.866686,   0.579250,   0.418100,
0.059901,   0.954740,   0.699338,   0.681427,
0.127995,   0.981347,   0.713651,   0.485167,
0.269875,   0.723569,   0.778578,   0.130682];

[U, S, V] = svd(A, 0)
 */

TEST(Main, SvdJacobiOneSided)
{
	// clang-format off
	float A[4 * 4] = {
		0.453690, 0.866686, 0.579250, 0.418100,
		0.059901, 0.954740, 0.699338, 0.681427,
		0.127995, 0.981347, 0.713651, 0.485167,
		0.269875, 0.723569, 0.778578, 0.130682
	};

	float U[4 * 4];
	float S[4];
	float V[4 * 4];

	float U_exp[6 * 6] = {
		-0.482323,   0.305658,   0.820263,   0.033267,
		-0.546082,  -0.627017,  -0.109543,   0.544650,
		-0.533155,  -0.191240,  -0.209917,  -0.796937,
		-0.430006,   0.690543,  -0.520677,   0.259116,
	};

	float S_exp[4] = {
		2.453708,
		0.424704,
		0.272155,
		0.066927,
	};

	float V_exp[4 * 4] = {
		-0.177619,   0.619249,   0.728252,   0.233734,
		-0.722880,  -0.051203,   0.086635,  -0.683606,
		-0.561012,   0.328978,  -0.575647,   0.495649,
		-0.362161,  -0.711113,   0.361627,   0.482062,
	};
	// clang-format on

	// Do SVD with one-sided jacobi
	svd_jacobi_one_sided(A, 4, 10, U, S, V);

	// Print U
	printf("U\n");
	print(U, 4, 4);

	// Print U
	printf("S\n");
	print(S, 4, 1);

	// Print V
	printf("V\n");
	print(V, 4, 4);

	/*
		// TODO: there is a flipped sign here but multiplication still turns out correct
	for (unsigned int c = 0; c < 4 * 4; c++) {
		ASSERT_NEAR(U_exp[c], U[c], 1e-3);
	}
	for (unsigned int c = 0; c < 4; c++) {
		ASSERT_NEAR(S_exp[c], S[c], 1e-3);
	}
	for (unsigned int c = 0; c < 4 * 4; c++) {
		ASSERT_NEAR(V_exp[c], V[c], 1e-3);
	}
	*/
	// verify A = U*S*V'
	float St[4 * 4];
	float SV[4 * 4];
	float Asvd[6 * 4];

	memset(St, 0, sizeof(St));
	for (unsigned int c = 0; c < 4; c++) {
		St[c * 4 + c] = S[c];
	}

	// SV = S*V'
	tran(V, V, 4, 4);

	mul(SV, St, V, 4, 4, 4, 4);
	// A = U * SV
	mul(Asvd, U, SV, 4, 4, 4, 4);

	for (unsigned int c = 0; c < 4 * 4; c++) {
		ASSERT_NEAR(A[c], Asvd[c], 1e-3);
	}
}
