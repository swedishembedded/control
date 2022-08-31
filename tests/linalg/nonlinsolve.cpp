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

/* Create a function on the form dx = Ax - b
	* b can contains ones if you want.
	* A need to be square, but can still have zero rows, the same for dx
	*/
void func(float *dx, float *b, float *x)
{
	dx[0] = 5 * x[0] + 2 * x[1] * x[2] + 9 * x[2] * x[2] - 50 * b[0];
	dx[1] = 10 * x[0] * x[1] * x[1] + 6 * x[1] - 3 * x[2] - 7 * b[1];
	dx[2] = 0 * x[0] - 0 * x[1] + 0 * x[2] - 0 * b[0];
}

TEST(Main, Nonlinsolve)
{
	// Initial parameters
	uint8_t elements = 3;
	bool random_guess_active = true;
	float x[3] = { 0, 0, 0 }; // This is our initial guess if random_guess_active = false
	float dx[3] = { 0, 0, 0 };
	float b[3] = { 1, 1, 1 };
	// Min max is only for random_guess_active
	float min_value = -5;
	float max_value = 5;
	float alpha = 0.05; // A smal positive number for tuning

	while (true) {
		// Do the process with random guess active
		nonlinsolve(func, b, x, elements, alpha, max_value, min_value, random_guess_active);
		func(dx, b, x);
		float accuracy = norm(dx, 1, elements, 2);
		if (accuracy > 0.001)
			continue;

		printf("Solution to func with random_guess_active = true:\n");
		printf("Accuracy(lower = better): %0.18f\n", accuracy);

		// Do the process with random guess inactive
		random_guess_active = false;
		x[0] = 1;
		x[1] = 1;
		x[2] = 1;
		alpha = 0.01;
		nonlinsolve(func, b, x, elements, alpha, max_value, min_value, random_guess_active);
		func(dx, b, x);
		accuracy = norm(dx, 1, elements, 2);
		if (accuracy > 0.001)
			continue;
		printf("Solution to func with random_guess_active = false:\n");
		printf("Accuracy(lower = better): %0.18f\n", accuracy);
		break;
	}
}
