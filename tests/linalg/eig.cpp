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

TEST(Main, EigenvalueCalculation)
{
	// Matrix A [4x4]
	float A[] = {
		0.018142, 0.968856, 0.151740, 0.757174, 0.017829, 0.474323, 0.358832, 0.970854,
		0.184523, 0.063063, 0.680511, 0.191901, 0.806877, 0.830208, 0.977169, 0.222291,
	};

	float wr_a_exp[4] = { 1.87922, -0.45009, -0.45009, 0.41623 };
	float wi_a_exp[4] = { 0, -0.11680, 0.11680, 0 };

	float wr_a[4]; // Eigenvalues real
	float wi_a[4]; // Eigenvalues imaginary

	// Matrix B
	float B[12 * 12] = {
		144, 2,	  3,   141, 140, 6,   7,   137, 136, 10,  11,  133, 13,	 131, 130, 16,
		17,  127, 126, 20,  21,	 123, 122, 24,	25,  119, 118, 28,  29,	 115, 114, 32,
		33,  111, 110, 36,  108, 38,  39,  105, 104, 42,  43,  101, 100, 46,  47,  97,
		96,  50,  51,  93,  92,	 54,  55,  89,	88,  58,  59,  85,  61,	 83,  82,  64,
		65,  79,  78,  68,  69,	 75,  74,  72,	73,  71,  70,  76,  77,	 67,  66,  80,
		81,  63,  62,  84,  60,	 86,  87,  57,	56,  90,  91,  53,  52,	 94,  95,  49,
		48,  98,  99,  45,  44,	 102, 103, 41,	40,  106, 107, 37,  109, 35,  34,  112,
		113, 31,  30,  116, 117, 27,  26,  120, 121, 23,  22,  124, 125, 19,  18,  128,
		129, 15,  14,  132, 12,	 134, 135, 9,	8,   138, 139, 5,   4,	 142, 143, 100,
	};

	float wr_b_exp[12] = { 879.11329, 108.51271, -117.23510, 98.60910, 0, 0, 0, 0, 0, 0, 0 };
	float wi_b_exp[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	float wr_b[12]; // Eigenvalues real
	float wi_b[12]; // Eigenvalues imaginary

	eig(A, wr_a, wi_a, 4);
	eig(B, wr_b, wi_b, 12);

	for (unsigned c = 0; c < 4; c++) {
		ASSERT_NEAR(wr_a_exp[c], wr_a[c], 1e-5);
		ASSERT_NEAR(wi_a_exp[c], wi_a[c], 1e-5);
	}

	for (unsigned c = 0; c < 12; c++) {
		ASSERT_NEAR(wr_b_exp[c], wr_b[c], 1e-3);
		ASSERT_NEAR(wi_b_exp[c], wi_b[c], 1e-3);
	}
}
