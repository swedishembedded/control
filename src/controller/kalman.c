// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include <control/linalg.h>
#include <control/controller.h>

/*
 * This is linear Kalman filter state update
 * x = Ax - KCx + Bu + Ky
 */
void kalman(float A[], float B[], float C[], float K[], float u[], float x[], float y[],
	    uint8_t ADIM, uint8_t YDIM, uint8_t RDIM)
{
	float Ax[ADIM * 1];
	float Bu[ADIM * 1];
	float Cx[YDIM * 1];
	float KCx[ADIM * 1];
	float Ky[ADIM * 1];

	mul(A, x, Ax, ADIM, ADIM, 1);
	mul(B, u, Bu, ADIM, RDIM, 1);
	mul(C, x, Cx, YDIM, ADIM, 1);
	mul(K, Cx, KCx, ADIM, YDIM, 1);
	mul(K, y, Ky, ADIM, YDIM, 1);

	// Estimate new discrete state
	// x = Ax + Bu + K * (y - Cx)
	for (uint8_t i = 0; i < ADIM; i++) {
		x[i] = Ax[i] + Bu[i] + Ky[i] - KCx[i];
	}
}
