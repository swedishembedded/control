// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"
#include "control/dynamics.h"

void kalman(float *xout, const float *const A, const float *x, const float *const B,
	    const float *const u, const float *const K, const float *const y, const float *const C,
	    uint8_t ADIM, uint8_t YDIM, uint8_t RDIM)
{
	float Ax[ADIM * 1];
	float Bu[ADIM * 1];
	float Cx[YDIM * 1];
	float KCx[ADIM * 1];
	float Ky[ADIM * 1];

	mul(Ax, A, x, ADIM, ADIM, ADIM, 1);
	mul(Bu, B, u, ADIM, RDIM, RDIM, 1);
	mul(Cx, C, x, YDIM, ADIM, ADIM, 1);
	mul(KCx, K, Cx, ADIM, YDIM, YDIM, 1);
	mul(Ky, K, y, ADIM, YDIM, YDIM, 1);

	// Estimate new discrete state
	// x = Ax + Bu + K * (y - Cx)
	for (uint8_t i = 0; i < ADIM; i++) {
		xout[i] = Ax[i] + Bu[i] + Ky[i] - KCx[i];
	}
}
