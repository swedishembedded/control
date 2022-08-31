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

static void integral(uint8_t ANTI_WINDUP, float xi[], float r[], float y[], uint8_t RDIM);

/*
 * This computes the Linear Quadratic Integral inputs
 * Measurement vector: y[YDIM]
 * Input signal vector: u[RDIM]
 * Integral scalar: qi
 * Reference vector: r[RDIM]
 * LQR control law L[RDIM*ADIM]
 * LQI integral law: Li[RDIM*ADIM]
 * State vector: x[ADIM]
 * Integral vector: xi[YDIM]
 */
void lqi(float y[], float u[], float qi, float r[], float L[], float Li[], float x[], float xi[],
	 uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t ANTI_WINDUP)
{
	// First compute the control law: L_vec = L*x
	float Lx[RDIM * 1];
	float Lixi[RDIM];

	mul(Lx, L, x, RDIM, ADIM, ADIM, 1);

	integral(ANTI_WINDUP, xi, r, y, RDIM);
	mul(Lixi, Li, xi, RDIM, YDIM, YDIM, 1);

	// Now combine these two laws: u = Li/(1-qi)*r - (L*x - Li*xi)
	for (uint8_t i = 0; i < RDIM; i++) {
		u[i] = (Li[i * RDIM] / (1 - qi) * r[i]) - (Lx[i] - Lixi[i]);
	}
}

/*
 * This computes the integral by sum state vector xi with reference - measurement
 * xi = xi + r - y;
 */
static void integral(uint8_t ANTI_WINDUP, float xi[], float r[], float y[], uint8_t RDIM)
{
	for (uint8_t i = 0; i < RDIM; i++) {
		/*
		 * Anti-windup
		 */
		if (ANTI_WINDUP == 0) {
			xi[i] = xi[i] + r[i] - y[i]; // Always integrate
		} else if (ANTI_WINDUP == 1) {
			if (r[i] > y[i]) {
				xi[i] = xi[i] + r[i] -
					y[i]; // Only integrate when r > y, else delete
			} else {
				xi[i] = 0; // Delete just that xi
			}
		} else if (ANTI_WINDUP == 2) {
			if (r[i] > y[i]) {
				xi[i] = xi[i] + r[i] -
					y[i]; // Only integrate r > y, else do nothing
			}
		} else {
			xi[i] = xi[i] + r[i] - y[i]; // Always integrate if nothing else selected
		}
	}
}
