/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/filter.h"
#include "control/misc.h"
};

TEST(Main, MCS)
{
	// Assume that we have a normal distributed vector with dimension row_x = 3
	uint8_t row_x = 3;
	float X0[2000];
	float X1[2000];
	float X2[2000];
	float x[3];
	uint16_t length = 2000;
	randn(X0, length, 20, 1); // Mean 20, Std 1
	randn(X1, length, -4, 1); // Mean -4, Std 1
	randn(X2, length, 6, 1); // Mean 6, Sted 1

	// Then we create a matrix P with row_x*2*column_p
	float P[6 * 500];
	uint16_t column_p =
		500; // You want to keep this small as possible, but still fill every index
	float index_factor =
		20; // Large number gives better precision but it requires larger column_p

	// Important to clean the P matrix first before filling P matrix with x vector.
	mcs_clean(P, column_p, row_x);
	for (uint16_t i = 0; i < length; i++) {
		x[0] = X0[i];
		x[1] = X1[i];
		x[2] = X2[i];
		mcs_collect(P, column_p, x, row_x, index_factor);
	}

	// Estimate which vector x is most applied in P
	mcs_estimate(P, column_p, x, row_x);

	// Print P and try to change index_factor and column_p for best fit.
	print(P, row_x * 2, column_p);

	// Don't forget to clean the filter before you are using it again
	mcs_clean(P, column_p, row_x);

	// Print best estimation
	printf("Final result: x[0] = %f, x[1] = %f, x[2] = %f\n", x[0], x[1], x[2]);
}
