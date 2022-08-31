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
#include "control/misc.h"
};

TEST(Main, OKID)
{
	// Input
	float u[2 * 10] = { 5, 1, 2, 3, 5, 1, 5, 6, 8, 1, 7, 1, 56, 1, 5, 3, 1, 5, 6, 1 };
	// Output
	float y[2 * 10] = { 3, 51, 3, 2, 6, 7, 8, 9, 1, 4, 1, 3, 1, 4, 5, 1, 7, 8, 1, 10 };

	// Impulse response - Markov parameters
	float s[2 * 10];

	okid(u, y, s, 2, 10);

	printf("Markov parameters:\n");
	print(s, 2, 10);
}

/*
 * GNU Octave code:
sys = ss(0, [0 1; -3 -5], eye(2), eye(2));
u = [linspace(5, 5, 100) linspace(2, 1, 100)];
t = linspace(0, 20, 200);
y = lsim(sys, [u;2*u], t);
sysd = okid([u;2*u], y, t(2), 1); % Select 2
 */
