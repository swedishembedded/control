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
#include "control/sysid.h"
#include "control/misc.h"
};

/* Our transition function - This is the orifice equation Q = a*sqrt(P2 - P1) for hydraulics */
void G(float dw[], float x[], float w[])
{
	dw[0] = w[0] * sqrtf(x[1] - x[0]);

	/* We only need to use w[0] so we assume that w[1] and w[2] will become close to 1.0 */
	dw[1] = w[1] * x[1];
	dw[2] = w[2] * x[2];
}

TEST(Main, SQRUKFID)
{
	/*
	 * Hello! This is Square Root Uncented Kalman Filter (SR-UKF) for parameter estimation and this algorithm is successor of Uncented Kalman Filter(UKF)
	 * because UKF had some issues with the cholesky decomposition when it going to find the square root.
	 * SR-UKF come in two papers. The first paper and the second paper. The first paper does not re-compute sigma point matrix for the
	 * observability function H. The second paper re-compute sigma point matrix for the observability function H.
	 * In this algorithm, I assume that the output y and the state has the same dimension and observability function is just an identity matrix.
	 * That's because I want to minimize the input arguments for the SR-UKF function. Less turning parameters, function, constants and so on makes
	 * it easier to use SR-UKF.
	 */

	/* Initial parameters */
	uint8_t L = 3; // How many states we have
	float e = 0.1f; // Tuning factor for noise
	float alpha = 0.1f; // Alpha value - A small number like 0.01 -> 1.0
	float beta = 2.0f; // Beta value - Normally 2 for gaussian noise
	float Re[3 * 3] = {
		e, 0, 0, 0, e, 0, 0, 0, e
	}; // Initial noise covariance matrix - Recommended to use identity matrix
	float Sw[3 * 3] = {
		1, 0, 0, 0, 1, 0, 0, 0, 1
	}; // Initial covariance matrix - Recommended to use identity matrix
	float what[3] = { 0, 0, 0 }; // Estimated parameter vector
	float d[3] = { 0, 0, 0 }; // This is our measurement
	float x[3] = { 4.4f, 6.2f, 1.0f }; // State vector
	float lambda_rls =
		1.0f; // RLS forgetting parameter between 0.0 and 1.0, but very close to 1.0
	/* Do SR-UKF */
	float WHAT[100 * 3];
	float E[100 * 3];
	float noise[1];
	float dw[3];
	for (uint32_t i = 0; i < 100; i++) {
		/* Create 1 noise */
		randn(noise, 1, 0.0f, 1.0f);

		/* Assume that this is our measurement */
		d[0] = 5.0f + e * noise[0];

		/* This is just to make sure w[1] and w[2] becomes close to 1.0 */
		d[1] = x[1];
		d[2] = x[2];

		/* Estimate new parameter - We assume that our x state vector remains constant */
		sqr_ukf_id(d, what, Re, x, G, lambda_rls, Sw, alpha, beta, L);

		/* Save the estimated parameter */
		for (uint8_t j = 0; j < L; j++)
			WHAT[i * 3 + j] = what[j];

		/* Measure the error */
		G(dw, x, what);
		for (uint8_t j = 0; j < L; j++)
			E[i * 3 + j] = fabsf(d[j] - dw[j]);
	}

	printf("Estimated parameters: \n");
	print(WHAT, 100, 3);

	printf("Error: \n");
	print(E, 100, 3);

	EXPECT_NEAR(E[99 * 3 + 0], 0, 0.4);
	EXPECT_NEAR(E[99 * 3 + 1], 0, 1e-3);
	EXPECT_NEAR(E[99 * 3 + 2], 0, 1e-3);
}
