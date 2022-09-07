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
 * Computes the determinant of square matrix A
 * A [m*n]
 * n == m
 * Return: Determinant value, or 0 for singular matrix
 */
float det(const float *const A, uint16_t row)
{
	float determinant = 1.0f;
	float LU[row * row];
	uint8_t P[row];

	if (lup(A, LU, P, row) != 0) {
		// LU decomposition failed
		// matrix is singular
		return 0;
	}

	for (uint16_t i = 0; i < row; ++i)
		determinant *= LU[row * P[i] + i];

	int j = 0;

	for (uint16_t i = 0; i < row; ++i)
		if (P[i] != i)
			++j;

	if (j && (j - 1) % 2 == 1)
		determinant = -determinant;

	return determinant;
}
