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

/*
 * GNU Octave code:
 * A = [0.9889143,   0.8156388,   0.7197012,   0.9117410,
	    0.3513571,   0.9509832,   0.7666883,   0.2984587,
		0.7419696,   0.6665918,   0.8349609,   0.0493249,
		0.5387147,   0.9922028,   0.7225777,   0.1423897,
	    0.0217779,   0.5598255,   0.5004498,   0.5730017,
		0.9608793,   0.4709475,   0.0031274,   0.0698641];

	b = [0.883369,
	     0.653627,
		 0.417359,
		 0.049459,
		 0.879004,
		 0.152040];

	alpha = 0.1;
    x = inv(A'*A + eye(size(A'*A))*alpha)*A'*b
 *
 */

TEST(Main, LinsolveGauss)
{
	// Matrix A
	// clang-format off
	float A[6 * 4] = {
		0.9889143, 0.8156388, 0.7197012, 0.9117410,
		0.3513571, 0.9509832, 0.7666883, 0.2984587,
		0.7419696, 0.6665918, 0.8349609, 0.0493249,
		0.5387147, 0.9922028, 0.7225777, 0.1423897,
		0.0217779, 0.5598255, 0.5004498, 0.5730017,
		0.9608793, 0.4709475, 0.0031274, 0.0698641 };

	// Vector b
	float b[6] = { 0.883369, 0.653627, 0.417359, 0.049459, 0.879004, 0.152040 };
	// clang-format on

	// Solution that need to have the same rows and columns from A
	float x[4];

	float x_exp[4] = { -0.044782, 0.065978, 0.305999, 0.792570 };
	// If we are using alpha = 0, then matrix A MUST be square!
	float alpha = 0.1;

	linsolve_gauss(A, x, b, 6, 4, alpha);

	for (unsigned int c = 0; c < 4; c++) {
		EXPECT_NEAR(x_exp[c], x[c], 1e-3);
	}
}
