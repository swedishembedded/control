/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <math.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/filter.h"
#include "control/misc.h"
};

/* Our transition function */
void F(float dx[], float x[], float u[])
{
	dx[0] = x[1];
	dx[1] = x[2];
	dx[2] = 0.05 * x[0] * (x[1] + x[2]);
}

TEST(Main, SRUKF)
{
	/*
	 * Hello! This is Square Root Uncented Kalman Filter (SR-UKF) for state estimation and this algorithm is successor of Uncented Kalman Filter(UKF)
	 * because UKF had some issues with the cholesky decomposition when it going to find the square root.
	 * SR-UKF come in two papers. The first paper and the second paper. The first paper does not re-compute sigma point matrix for the
	 * observability function H. The second paper re-compute sigma point matrix for the observability function H.
	 * In this algorithm, I assume that the output y and the state has the same dimension and observability function is just an identity matrix.
	 * That's because I want to minimize the input arguments for the SR-UKF function. Less turning parameters, function, constants and so on makes
	 * it easier to use SR-UKF.
	 */

	/* Initial parameters */
	// How many states we have
	uint8_t L = 3;
	// Tuning factor for noise
	float r = 0.1f;
	// Tuning factor for disturbance
	float q = 0.01f;
	// Alpha value - A small number like 0.01 -> 1.0
	float alpha = 0.1f;
	// Beta value - Normally 2 for gaussian noise
	float beta = 2.0f;
	// Initial disturbance covariance matrix - Recommended to use identity matrix
	float Rv[3 * 3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	// Initial noise covariance matrix - Recommended to use identity matrix
	float Rn[3 * 3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	// Initial covariance matrix - Recommended to use identity matrix
	float S[3 * 3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	float xhat[3] = { 0, 0, 1 }; // Estimated state vector
	float y[3] = { 0, 0, 0 }; // This is our measurement
	float u[3] = {
		0, 0, 0
	}; // u is not used in this example due to the transition function not using an input signal
	float x[3] = { 0, 0, 0 }; // State vector for the system (unknown in reality)

	/* Save arrays */
	float X[200 * 3];
	float XHAT[200 * 3];
	float Y[200 * 3];

	/* Measurement noise */
	float noise[3];

	/* Disturbances */
	float phase[3] = { 90, 180, 140 };
	float amplitude[3] = { 0.1f, 0.1f, 0.1f };

	/* Derivative */
	float dx[3];

	/* SR-UKF simulation */
	for (uint32_t i = 0; i < 200; i++) {
		/* Create noise for the measurement */
		randn(noise, L, 0.0f, 1.0f);
		for (uint8_t j = 0; j < L; j++)
			y[j] = x[j] + r * noise[j];

		/* Save measurement */
		for (uint8_t j = 0; j < L; j++)
			Y[i * 3 + j] = y[j];

		/* Save actual state */
		for (uint8_t j = 0; j < L; j++)
			X[i * 3 + j] = x[j];

		/* Estimate new state */
		sqr_ukf(y, xhat, Rn, Rv, u, F, S, alpha, beta, L);

		/* Save the estimated state */
		for (uint8_t j = 0; j < L; j++) {
			XHAT[i * 3 + j] = xhat[j];
		}

		/* Update the state with disturbance */
		F(dx, x, u);
		for (uint8_t j = 0; j < L; j++) {
			x[j] = dx[j] + q * amplitude[j] * sinf(i + phase[j]);
		}
	}

	/* Data */
	printf("Actual state:\n");
	print(X, 200, 3);

	printf("Estimated state:\n");
	print(XHAT, 200, 3);

	printf("Measurement:\n");
	print(Y, 200, 3);
}
