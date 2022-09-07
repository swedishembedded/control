// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/dynamics.h"
#include "control/linalg.h"
#include "control/optimization.h"

#include <errno.h>
#include <string.h>

/*
 * [C*A^1; C*A^2; C*A^3; ... ; C*A^HORIZON] % Extended observability matrix
 */
static void obsv(float PHI[], float A[], float C[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM,
		 uint8_t HORIZON)
{
	(void)RDIM;
	// This matrix will A^(i+1) all the time
	float A_copy[ADIM * ADIM];

	memcpy(A_copy, A, ADIM * ADIM * sizeof(float));

	// Temporary matrix
	float T[YDIM * ADIM];
	//memset(T, 0, YDIM * ADIM * sizeof(float));

	// Regular T = C*A^(1+i)
	mul(T, C, A, YDIM, ADIM, ADIM, ADIM);

	// Insert temporary T into PHI
	memcpy(PHI, T, YDIM * ADIM * sizeof(float));

	// Do the rest C*A^(i+1) because we have already done i = 0
	float A_pow[ADIM * ADIM];

	for (uint8_t i = 1; i < HORIZON; i++) {
		mul(A_pow, A, A_copy, ADIM, ADIM, ADIM, ADIM); //  Matrix power A_pow = A*A_copy
		mul(T, C, A_pow, YDIM, ADIM, ADIM, ADIM); // T = C*A^(1+i)
		memcpy(PHI + i * YDIM * ADIM, T,
		       YDIM * ADIM * sizeof(float)); // Insert temporary T into PHI
		memcpy(A_copy, A_pow, ADIM * ADIM * sizeof(float)); // A_copy <- A_pow
	}
}

/*
 * Lower triangular toeplitz of extended observability matrix
 * CAB stands for C*A^i*B because every element is C*A*B
 */
static void cab(float GAMMA[], float PHI[], const float *const A, float B[], float C[],
		uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t HORIZON)
{
	(void)A;
	// First create the initial C*A^0*B == C*I*B == C*B
	float CB[YDIM * RDIM];

	mul(CB, C, B, YDIM, ADIM, ADIM, RDIM);

	// Take the transpose of CB so it will have dimension RDIM*YDIM instead
	tran(CB, CB, YDIM, RDIM);

	// Create the CAB matrix from PHI*B
	float PHIB[HORIZON * YDIM * RDIM];

	mul(PHIB, PHI, B, HORIZON * YDIM, ADIM, ADIM, RDIM); // CAB = PHI*B
	tran(PHIB, PHIB, HORIZON * YDIM, RDIM);

	/*
	 * We insert GAMMA = [CB PHI;
	 *                    0  CB PHI;
	 *                    0   0  CB PHI;
	 *                    0   0   0  CB PHI] from left to right
	 */
	for (uint8_t i = 0; i < HORIZON; i++) {
		for (uint8_t j = 0; j < RDIM; j++) {
			memcpy(GAMMA + HORIZON * YDIM * (i * RDIM + j) + YDIM * i, CB + YDIM * j,
			       YDIM * sizeof(float)); // Add CB
			memcpy(GAMMA + HORIZON * YDIM * (i * RDIM + j) + YDIM * i + YDIM,
			       PHIB + HORIZON * YDIM * j,
			       (HORIZON - i - 1) * YDIM * sizeof(float)); // Add PHI*B
		}
	}

	// Transpose of gamma
	tran(GAMMA, GAMMA, HORIZON * RDIM, HORIZON * YDIM);
}

int mpc(float A[], float B[], float C[], float x[], float u[], const float *const r, uint8_t ADIM,
	uint8_t YDIM, uint8_t RDIM, uint8_t HORIZON, uint8_t ITERATION_LIMIT, bool has_integration)
{
	if (HORIZON == 0) {
		// Horizon can not be zero!
		return -EINVAL;
	}
	if (YDIM == 0) {
		// we must have measurement!
		return -EINVAL;
	}
	if (RDIM == 0) {
		// we must have reference and output dimension
		return -EINVAL;
	}

	// Create the extended observability matrix
	float PHI[HORIZON * YDIM * ADIM];

	obsv(PHI, A, C, ADIM, YDIM, RDIM, HORIZON);

	// Create the lower triangular toeplitz matrix
	float GAMMA[HORIZON * YDIM * HORIZON * RDIM];

	// We need memset here
	memset(GAMMA, 0, HORIZON * YDIM * HORIZON * RDIM * sizeof(float));
	cab(GAMMA, PHI, A, B, C, ADIM, YDIM, RDIM, HORIZON);

	// Find the input value from GAMMA and PHI
	// R_vec = R*r
	float R_vec[HORIZON * YDIM];

	for (uint8_t i = 0; i < HORIZON * YDIM; i++) {
		for (uint8_t j = 0; j < YDIM; j++) {
			R_vec[i + j] = r[j];
		}
		i += YDIM - 1;
	}

	// PHI_vec = PHI*x
	float PHI_vec[HORIZON * YDIM];

	mul(PHI_vec, PHI, x, HORIZON * YDIM, ADIM, ADIM, 1);

	// R_PHI_vec = R_vec - PHI_vec
	float R_PHI_vec[HORIZON * YDIM];

	for (uint8_t i = 0; i < HORIZON * YDIM; i++) {
		*(R_PHI_vec + i) = *(R_vec + i) - *(PHI_vec + i);
	}

	// Transpose gamma
	float GAMMAT[HORIZON * YDIM * HORIZON * RDIM];

	memcpy(GAMMAT, GAMMA, HORIZON * YDIM * HORIZON * RDIM * sizeof(float)); // GAMMA -> GAMMAT
	tran(GAMMAT, GAMMAT, HORIZON * YDIM, HORIZON * RDIM);

	// b = GAMMAT*R_PHI_vec
	float b[HORIZON * YDIM];

	//memset(b, 0, HORIZON * YDIM * sizeof(float));
	mul(b, GAMMAT, R_PHI_vec, HORIZON * RDIM, HORIZON * YDIM, HORIZON * YDIM, 1);

	// GAMMATGAMMA = GAMMAT*GAMMA = A
	float GAMMATGAMMA[HORIZON * RDIM * HORIZON * RDIM];

	//memset(GAMMATGAMMA, 0, HORIZON * RDIM*HORIZON * RDIM * sizeof(float));
	mul(GAMMATGAMMA, GAMMAT, GAMMA, HORIZON * RDIM, HORIZON * YDIM, HORIZON * YDIM,
	    HORIZON * RDIM);

	// Copy A and call it AT
	float AT[HORIZON * RDIM * HORIZON * RDIM];

	memcpy(AT, GAMMATGAMMA, HORIZON * RDIM * HORIZON * RDIM * sizeof(float)); // A -> AT
	tran(AT, AT, HORIZON * RDIM, HORIZON * RDIM);

	// Now create c = AT*R_PHI_vec
	float c[HORIZON * YDIM];

	mul(c, AT, R_PHI_vec, HORIZON * RDIM, HORIZON * RDIM, HORIZON * RDIM, 1);

	// Do linear programming now
	linprog(c, GAMMATGAMMA, b, R_vec, HORIZON * YDIM, HORIZON * RDIM, 0, ITERATION_LIMIT);

	// We select the best input values, depending on if we have integration behavior or not in our model
	if (has_integration == true) {
		// Set first R_vec to u - Done
		for (uint8_t i = 0; i < RDIM; i++) {
			u[i] = R_vec[i];
		}
	} else {
		// Set last R_vec to u - Done
		for (uint8_t i = 0; i < RDIM; i++) {
			u[i] = R_vec[HORIZON * RDIM - RDIM + i];
		}
	}

	return 0;
}
