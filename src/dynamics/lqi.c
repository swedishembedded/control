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

/*
 * This computes the integral by sum state vector xi with error (ref - measurement)
 * xi = xi + err;
 */
static void integral(uint8_t ANTI_WINDUP, float *xi, const float *const err, uint8_t RDIM)
{
	for (uint8_t i = 0; i < RDIM; i++) {
		/*
		 * TODO: this antiwindup looks fishy fishy fishy (investigate)
		 * Anti-windup
		 */
		if (ANTI_WINDUP == 1) {
			if (err[i] < 0) {
				// Only integrate when r > y, else delete
				xi[i] = xi[i] + err[i];
			} else {
				xi[i] = 0; // Delete just that xi
			}
		} else if (ANTI_WINDUP == 2) {
			if (err[i] < 0) {
				// Only integrate r > y, else do nothing
				xi[i] = xi[i] + err[i];
			}
		} else {
			// Always integrate if nothing else selected
			xi[i] = xi[i] + err[i];
		}
	}
}

void lqi(float *u, const float *const L, const float *const x, const float *const Li, float *xi,
	 const float *const err, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t ANTI_WINDUP)
{
	// First compute the control law: L_vec = L*x
	float Lx[RDIM * 1];
	float Lixi[RDIM];

	mul(Lx, L, x, RDIM, ADIM, ADIM, 1);
	integral(ANTI_WINDUP, xi, err, RDIM);
	mul(Lixi, Li, xi, RDIM, YDIM, YDIM, 1);

	// Now comput the control action
	for (uint8_t i = 0; i < RDIM; i++) {
		u[i] = -(Lx[i] + Lixi[i]);
	}
}
