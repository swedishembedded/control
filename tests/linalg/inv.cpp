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

TEST(Main, MatrixInverse)
{
	// clang-format: off
	float A[3 * 3] = { 0, 1, 2, 1, 2, 0, 2, 0, 0 };
	float Ai_exp[3 * 3] = { -0.00000, -0.00000, 0.50000,  -0.00000, 0.50000,
				-0.25000, 0.50000,  -0.25000, 0.12500 };
	// clang-format: on

	ASSERT_EQ(0, inv(A, A, 3));

	for (unsigned c = 0; c < 3 * 3; c++) {
		ASSERT_NEAR(Ai_exp[c], A[c], 1e-5);
	}
}

TEST(Main, MatrixInverse4x4)
{
	// clang-format off
	float A[4 * 4] = {
		0.018142, 0.968856, 0.151740, 0.757174,
		0.017829, 0.474323, 0.358832, 0.970854,
		0.184523, 0.063063, 0.680511, 0.191901,
		0.806877, 0.830208, 0.977169, 0.222291 };

	float Ai_exp[4 * 4] = {
		-2.68276, 2.33073, -3.60781, 2.07323,
		1.87926, -1.55277, 0.61217, -0.14794,
		0.89119, -1.09449, 2.74252, -0.62304,
		-1.19826, 2.15037,-1.24648, 0.26448 };
	// clang-format on

	ASSERT_EQ(0, inv(A, A, 4));

	for (unsigned c = 0; c < 4 * 4; c++) {
		ASSERT_NEAR(Ai_exp[c], A[c], 1e-5);
	}
}

TEST(Main, MatrixInverse12x12)
{
	// clang-format off
	float A[12 * 12] = { 144, 2,   3,   141, 140, 6,   7,	137, 136, 10, 11,  133, 13,  131,
			     130, 16,  17,  127, 126, 20,  21,	123, 122, 24, 25,  119, 118, 28,
			     29,  115, 114, 32,	 33,  111, 110, 36,  108, 38, 39,  105, 104, 42,
			     43,  101, 100, 46,	 47,  97,  96,	50,  51,  93, 92,  54,	55,  89,
			     88,  58,  59,  85,	 61,  83,  82,	64,  65,  79, 78,  68,	69,  75,
			     74,  72,  73,  71,	 70,  76,  77,	67,  66,  80, 81,  63,	62,  84,
			     60,  86,  87,  57,	 56,  90,  91,	53,  52,  94, 95,  49,	48,  98,
			     99,  45,  44,  102, 103, 41,  40,	106, 107, 37, 109, 35,	34,  112,
			     113, 31,  30,  116, 117, 27,  26,	120, 121, 23, 22,  124, 125, 19,
			     18,  128, 129, 15,	 14,  132, 12,	134, 135, 9,  8,   138, 139, 5,
			     4,	  142, 143, 1 };

	float Ai_exp[12 * 12] = {
		6.4748e+13,  1.4939e+12,  1.2664e+14,  -5.1719e+13, 9.4063e+13,	 -4.4066e+13,
		1.2117e+14,  1.4218e+14,  -1.3937e+14, -1.8341e+14, -2.1831e+13, -1.0990e+14,
		6.1222e+12,  2.2056e+13,  -1.7393e+13, 2.8059e+13,  -2.1724e+13, 1.6644e+13,
		3.6674e+13,  3.3087e+13,  -1.2816e+13, -5.5624e+13, -2.3561e+12, -3.2728e+13,
		-9.5544e+12, -3.6528e+13, 6.8986e+13,  -8.7812e+12, 7.2922e+13,	 -3.1947e+13,
		5.8627e+13,  3.6877e+13,  -8.2153e+13, -6.1760e+13, 2.6213e+12,	 -9.3102e+12,
		-4.8626e+13, -5.0342e+12, -1.3053e+14, -1.0955e+13, -4.4274e+13, 6.1313e+13,
		-6.7670e+13, -6.0372e+13, 9.2895e+13,  1.5840e+14,  -1.6484e+13, 7.1331e+13,
		-3.5234e+13, -3.0490e+12, 8.6423e+12,  8.9678e+13,  -6.7673e+13, -9.7194e+12,
		-1.6749e+13, -2.8045e+12, 1.0377e+13,  -5.0057e+13, 7.0932e+13,	 5.6565e+12,
		4.7129e+12,  -7.6754e+12, 9.9511e+13,  -3.2792e+13, 7.8186e+13,	 -8.3994e+13,
		3.0038e+13,  7.1028e+13,  -7.8134e+13, -4.0675e+13, 2.7942e+12,	 -4.3002e+13,
		-3.0378e+12, 1.6777e+13,  -1.0679e+14, 1.3514e+13,  -1.0435e+14, 8.7103e+13,
		-1.0487e+14, -6.5937e+13, 1.1446e+14,  1.1115e+14,  -3.3658e+12, 4.5351e+13,
		6.5671e+13,  5.2025e+12,  -1.0125e+12, -2.8910e+13, -7.2491e+13, -6.7023e+12,
		-3.9843e+13, -7.0941e+13, 7.3667e+13,  7.5060e+13,  -3.2705e+13, 3.3004e+13,
		-1.1233e+14, -1.1547e+13, 2.5087e+13,  7.2275e+13,  1.4575e+14,	 -3.3368e+13,
		1.7669e+13,  -2.2794e+13, -9.0344e+13, -0.0000e+00, 2.1580e+12,	 7.4410e+12,
		1.2189e+13,  5.9748e+12,  1.4773e+13,  0.0000e+00,  -7.2937e+12, -1.1829e+13,
		3.8097e+13,  2.5018e+13,  1.1728e+13,  -4.6912e+13, -1.0212e+11, -4.1642e+13,
		-1.0432e+13, -6.0471e+11, -5.9091e+13, 0.0000e+00,  -1.7738e+13, 2.4023e+13,
		-5.8561e+13, -1.0007e+14, 4.6912e+13,  9.3825e+13,  4.0846e+11,	 8.1330e+13,
		6.5773e+13,  1.2933e+13,  -2.8831e+13, -7.0369e+13, -5.5378e+13, 3.2542e+13,
		-1.4574e+13, 1.4733e+13,  5.2777e+13,  0.0000e+00,  -2.0707e+12, -7.5360e+12,
	};
	// clang-format on

	// this is a singular matrix (det == -2.4325e-119)
	ASSERT_EQ(-ENOTSUP, inv(A, A, 12));
}
