/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Copyright 2017 Daniel Mårtensson
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <math.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/dynamics.h"
#include "control/linalg.h"
};

TEST(Main, PID)
{
	// create a simple system with time constant 0.4 and gain 4
	// G = 4/(0.4*s + 1)
	// c2d(ss(G), 1)
	float A[] = { 0.778800783 };
	float B[] = { 0.884796868 };
	float C[] = { 1 };
	float K[] = { 0 };
	float r[] = { 4 };
	float y[] = { 0 };
	float u[] = { 0 };
	float x[] = { 0 };
	float Ts = 0.1;

	struct pid pid;
	pid_init(&pid);
	//pid_set_from_imc(&pid, 0.2, 4, 0.4, 0);
	pid_set_gains(&pid, 12.5, 3.125, 0, 0);

	for (unsigned int c = 0; c < 4; c++) {
		u[0] = pid_step(&pid, (r[0] - y[0]) * Ts);
		kalman(x, A, x, B, u, K, y, C, 1, 1, 1);
		mul(y, C, x, 1, 1, 1, 1);
	}

	// 0.6326 -> 63% of full scale
	float tau = 1.0f - expf(-1.f);

	// check that the system is following precisly its time constant
	EXPECT_NEAR(r[0] * tau, y[0] * tau, 0.05);
}
