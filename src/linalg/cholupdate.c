// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"

#include <math.h>
#include <string.h>

/*
 * Create L = cholupdate(L, x, rank_one_update)
 * L is a lower triangular matrix with real and positive diagonal entries from cholesky decomposition L = chol(A)
 * L [m*n]
 * x [m]
 * n == m
 */
void cholupdate(float *L, const float *const xx, uint16_t row, bool rank_one_update)
{
	float alpha = 0.0f, beta = 1.0f, beta2 = 0.0f, gamma = 0.0f, delta = 0.0f;
	float x[row];

	memcpy(x, xx, sizeof(x));

	tran(L, L, row, row);

	for (int i = 0; i < row; i++) {
		alpha = x[i] / L[row * i + i];
		beta2 = rank_one_update == true ? sqrtf(beta * beta + alpha * alpha) :
						  sqrtf(beta * beta - alpha * alpha);
		gamma = rank_one_update == true ? alpha / (beta2 * beta) : -alpha / (beta2 * beta);

		if (rank_one_update) {
			// Update
			delta = beta / beta2;
			L[row * i + i] = delta * L[row * i + i] + gamma * x[i];

			if (i < row) {
				// line 34 in tripfield chol_updown function
				for (int k = i + 1; k < row; k++)
					x[k] -= alpha * L[row * k + i];

				// line 35 in tripfield chol_updown function
				for (int k = i + 1; k < row; k++)
					L[row * k + i] = delta * L[row * k + i] + gamma * x[k];
			}
			x[i] = alpha;
			beta = beta2;
		} else {
			// Downdate
			delta = beta2 / beta;
			L[row * i + i] = delta * L[row * i + i];

			if (i < row) {
				for (int k = i + 1; k < row; k++) {
					x[k] -= alpha * L[row * k + i];
				}

				for (int k = i + 1; k < row; k++) {
					L[row * k + i] = delta * L[row * k + i] + gamma * x[k];
				}
			}
			x[i] = alpha;
			beta = beta2;
		}
	}

	tran(L, L, row, row);
}
