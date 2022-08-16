// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include <control/linalg.h>

/*
 * Solve Ax=b with QR decomposition
 * A[m*n]
 * b[m]
 * x[n]
 */
void linsolve_qr(float A[], float x[], float b[], uint16_t row, uint16_t column)
{
	// QR-decomposition
	float Q[row * row];
	float R[row * column];
	float QTb[row];

	qr(A, Q, R, row, column, false);
	tran(Q, row, row); // Do transpose Q -> Q^T
	mul(Q, b, QTb, row, row, 1); // Q^Tb = Q^T*b
	linsolve_upper_triangular(R, x, QTb, column);
}
