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
#include "control/sysid.h"
#include "control/linalg.h"
#include "control/misc.h"
};

TEST(Main, Solve1D)
{
	// clang-format off
	float u[5] = { 1, 2, 3, 4, 5 };
	float y[5] = { 5, 4, 3, 2, 1 };
	float t[5 * 5] = {
		1, 0, 0, 0, 0,
		2, 1, 0, 0, 0,
		3, 2, 1, 0, 0,
		4, 3, 2, 1, 0,
		5, 4, 3, 2, 1
	};
	float y_hat[5];
	// clang-format on

	// Impulse response - Markov parameters
	float s[5];

	linsolve_markov(s, y, u, 1, 5);

	printf("Markov parameters:\n");
	print(s, 1, 5);

	mul(y_hat, t, s, 5, 5, 5, 1);

	for (int c = 0; c < 5; c++) {
		EXPECT_NEAR(y_hat[c], y[c], 1e-6);
	}
}

TEST(Main, Solve1x10)
{
	// clang-format off
	float u[10] = {
		5, 1, 2,  3, 5, 1, 5, 6, 8, 1,
	};
	float y[10] = {
		3, 51, 3, 2, 6, 7, 8, 9, 1, 4,
	};
	float t[10 * 10] = {
		5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 5, 0, 0, 0, 0, 0, 0, 0, 0,
		2, 1, 5, 0, 0, 0, 0, 0, 0, 0,
		3, 2, 1, 5, 0, 0, 0, 0, 0, 0,
		5, 3, 2, 1, 5, 0, 0, 0, 0, 0,
		1, 5, 3, 2, 1, 5, 0, 0, 0, 0,
		5, 1, 5, 3, 2, 1, 5, 0, 0, 0,
		6, 5, 1, 5, 3, 2, 1, 5, 0, 0,
		8, 6, 5, 1, 5, 3, 2, 1, 5, 0,
		1, 8, 6, 5, 1, 5, 3, 2, 1, 5,
	};
	float y_hat[10];
	// clang-format on

	// Impulse response - Markov parameters
	float s[10];

	linsolve_markov(s, y, u, 1, 10);

	printf("Markov parameters:\n");
	print(s, 1, 10);

	mul(y_hat, t, s, 10, 10, 10, 1);
	printf("Y:\n");
	print(y, 1, 10);
	printf("Y_hat:\n");
	print(y_hat, 1, 10);

	for (int c = 0; c < 10; c++) {
		EXPECT_NEAR(y_hat[c], y[c], 1e-5);
	}
}

TEST(Main, Solve2x10)
{
	// clang-format off
	float u[2 * 10] = {
		5, 1, 2,  3, 5, 1, 5, 6, 8, 1,
		7, 1, 56, 1, 5, 3, 1, 5, 6, 1
	};
	float y[2 * 10] = {
		3, 51, 3, 2, 6, 7, 8, 9, 1, 4,
		1, 3, 1, 4, 5, 1, 7, 8, 1, 10
	};
	float y_hat[2 * 10];
	float t[20 * 10] = {
		// first row
		5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 5, 0, 0, 0, 0, 0, 0, 0, 0,
		2, 1, 5, 0, 0, 0, 0, 0, 0, 0,
		3, 2, 1, 5, 0, 0, 0, 0, 0, 0,
		5, 3, 2, 1, 5, 0, 0, 0, 0, 0,
		1, 5, 3, 2, 1, 5, 0, 0, 0, 0,
		5, 1, 5, 3, 2, 1, 5, 0, 0, 0,
		6, 5, 1, 5, 3, 2, 1, 5, 0, 0,
		8, 6, 5, 1, 5, 3, 2, 1, 5, 0,
		1, 8, 6, 5, 1, 5, 3, 2, 1, 5,
		// second row
		7, 0,  0, 0, 0, 0, 0, 0, 0, 0,
		1, 7,  0, 0, 0, 0, 0, 0, 0, 0,
		56,1,  7, 0, 0, 0, 0, 0, 0, 0,
		1, 56, 1, 7, 0, 0, 0, 0, 0, 0,
		5, 1, 56, 1, 7, 0, 0, 0, 0, 0,
		3, 5, 1, 56, 1, 7, 0, 0, 0, 0,
		1, 3, 5, 1, 56, 1, 7, 0, 0, 0,
		5, 1, 3, 5, 1, 56, 1, 7, 0, 0,
		6, 5, 1, 3, 5, 1, 56, 1, 7, 0,
		1, 6, 5, 1, 3, 5, 1, 56, 1, 7,
	};
	// clang-format on

	// Impulse response - Markov parameters
	float s[2 * 10];

	linsolve_markov(s, y, u, 2, 10);

	printf("Markov parameters:\n");
	print(s, 2, 10);

	mul(y_hat, t, s, 10, 10, 10, 1);
	mul(y_hat + 10, t + 10 * 10, s + 10, 10, 10, 10, 1);

	printf("y_hat:\n");
	print(y_hat, 2, 10);

	for (int c = 0; c < 2 * 10; c++) {
		EXPECT_NEAR(y_hat[c], y[c], 1e-5);
	}

	// clang-format off
	float s_exp[2 * 10] = {
		0.600000f, 10.080000f, -1.656000f, -3.660800f, -4.053440f, -5.531393f, 5.564134f,  -1.476206f,  -5.025977f, -5.200722f,
		0.142857f, 0.408163f,  -1.058309f, -2.563099f, 9.386564f,  19.105127f, -75.895058f,-140.072937f,618.715332f,1027.548828
	};
	// clang-format on

	for (int c = 0; c < 2 * 10; c++) {
		EXPECT_NEAR(s_exp[c], s[c], 1e-6);
	}
}
