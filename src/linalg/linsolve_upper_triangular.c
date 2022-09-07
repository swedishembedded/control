// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/linalg.h"

#include <string.h>

/*
 * This solves Ax = b.
 * A need to be square and upper triangular.
 * A [m*n]
 * b [m]
 * m == n
 */
void linsolve_upper_triangular(const float *const A, float x[], const float *const b,
			       uint16_t column)
{
	// Time to solve x from Ax = b.
	memset(x, 0, column * sizeof(float));

	// Column
	for (int i = column - 1; i >= 0; i--) {
		float sum = 0.0f; // This is our sum
		// Row
		for (int j = i; j < column; j++) {
			sum += A[i * column + j] * x[j];
		}
		x[i] = (b[i] - sum) / A[i * column + i];

		if (i == 0) {
			break;
		}
	}
}

/*
 * GNU Octave code:
 *  function [x] = linsolve(A, b)
	  s = length(A);
	  x = zeros(s,1);
	  x(s) = b(s)/A(s,s);
	  for i = s-1:-1:1
		  sum = 0;
		  for j = s:-1:i+1
			  sum = sum + A(i,j)*x(j);
		  end
		  x(i) = (b(i)- sum)/A(i,i);
	  end
	end
 */
