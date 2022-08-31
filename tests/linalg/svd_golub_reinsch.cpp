/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Copyright 2017 Daniel Mårtensson
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <cmath>
#include <gtest/gtest.h>

extern "C" {
#include "control/linalg.h"
#include "control/misc.h"
};

/*
 * GNU Octave code:
A = [0.7179787,   0.7985186,   0.1000046,   0.2203064,
0.9044292,   0.5074379,   0.3539301,   0.9475452,
0.0029252,   0.4930148,   0.3209303,   0.5289174,
0.6546133,   0.7354447,   0.9989453,   0.0310190,
0.7434944,   0.0874402,   0.3388867,   0.8256180,
0.7483093,   0.3624991,   0.2039784,   0.5528368];

[U, S, V] = svd(A, 0)
 */

TEST(Main, SvdGolubReinsch)
{
	// clang-format off
	float A[6 * 4] = {
		0.7179787, 0.7985186, 0.1000046, 0.2203064,
		0.9044292, 0.5074379, 0.3539301, 0.9475452,
		0.0029252, 0.4930148, 0.3209303, 0.5289174,
		0.6546133, 0.7354447, 0.9989453, 0.0310190,
		0.7434944, 0.0874402, 0.3388867, 0.8256180,
		0.7483093, 0.3624991, 0.2039784, 0.5528368
	};

	float U[6 * 4];
	float S[4];
	float V[4 * 4];

	float U_exp[6 * 4] = {
		-0.3750814,  -0.2117712,  -0.7836340,   0.1960256,
		-0.5451732,   0.3326720,   0.0105756,   0.0674080,
		-0.2363487,  -0.0347335,   0.3694144,   0.8653948,
		-0.4413482,  -0.7820001,   0.3335684,  -0.2829248,
		-0.4052826,   0.4438348,   0.3165472,  -0.3232176,
		-0.3836629,   0.1864465,  -0.1945993,  -0.1536418,
	};

	float S_exp[4] = {
		2.58621,
		0.99431,
		0.59299,
		0.52066,
	};

	float V_exp[4 * 4] = {
		-0.63429,   0.10694,  -0.41131,  -0.64581,
		-0.46082,  -0.48892,  -0.39764,   0.62489,
		-0.37228,  -0.51022,   0.74998,  -0.19650,
		-0.49672,   0.69943,   0.33202,   0.39221,
	};
	// clang-format on

	// Do SVD with Golub Reinsch method
	EXPECT_EQ(0, svd_golub_reinsch(A, 6, 4, U, S, V));

	/*
	// TODO: this currently does not work
	for (unsigned int c = 0; c < 6 * 6; c++) {
		ASSERT_NEAR(U_exp[c], U[c], 1e-3);
	}
	for (unsigned int c = 0; c < 4; c++) {
		ASSERT_NEAR(S_exp[c], S[c], 1e-3);
	}
	for (unsigned int c = 0; c < 4 * 4; c++) {
		ASSERT_NEAR(V_exp[c], V[c], 1e-3);
	}
	*/

	// Print U
	printf("U\n");
	print(U, 6, 4);

	// Print U
	printf("S\n");
	print(S, 4, 1);

	// Print V
	printf("V\n");
	print(V, 4, 4);

	// Print A
	printf("A\n");
	print(A, 6, 4);

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
	mul(Asvd, U, SV, 6, 4, 4, 4);

	for (unsigned int c = 0; c < 6 * 4; c++) {
		ASSERT_NEAR(A[c], Asvd[c], 1e-3);
	}
}
