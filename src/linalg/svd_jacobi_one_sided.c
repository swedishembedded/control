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

void svd_jacobi_one_sided(const float *const Ain, uint16_t row, uint8_t max_iterations, float *U,
			  float *S, float *V)
{
	// i and j are the indices of the point we've chosen to zero out
	float al, b, c;
	int i, j, p, k;
	float A[row * row];

	memcpy(A, Ain, sizeof(A));

	// Create the identity matrix
	memset(U, 0, row * row * sizeof(float));
	memset(V, 0, row * row * sizeof(float));
	memset(S, 0, row * sizeof(float));
	for (i = 0; i < row; i++) {
		*(U + row * i + i) = 1;
		*(V + row * i + i) = 1;
	}

	// Apply max_iterations times. That should be good enough
	for (p = 0; p < max_iterations; p++) {
		// For all pairs i < j
		for (i = 0; i < row; i++) {
			for (j = i + 1; j < row; j++) {
				al = b = c = 0.0f;
				// Find the 2x2 submatrix
				for (k = 0; k < row; k++) {
					al += *(A + row * k + i) * *(A + row * k + i);
					b += *(A + row * k + j) * *(A + row * k + j);
					c += *(A + row * k + i) * *(A + row * k + j);
				}

				// Compute Jacobi rotation
				float l = (b - al) / (2.0f * c);

				float sign = 1.0f;

				if (l < 0.0f)
					sign = -1.0f;

				float t = sign / ((sign * l) + sqrtf(1.0f + l * l));
				float cs = 1.0f / sqrtf(1.0f + t * t);
				float sn = cs * t;

				// Change columns i and j only
				for (k = 0; k < row; k++) {
					float tmp = *(A + row * k + i);

					*(A + row * k + i) = cs * tmp - sn * *(A + row * k + j);
					*(A + row * k + j) = sn * tmp + cs * *(A + row * k + j);
				}

				// Update the right singular vectors
				for (k = 0; k < row; k++) {
					float tmp = *(V + row * k + i);

					*(V + row * k + i) = cs * tmp - sn * *(V + row * k + j);
					*(V + row * k + j) = sn * tmp + cs * *(V + row * k + j);
				}
			}
		}
	}

	// Find the singular values by adding up squares of each column, then taking square root of each column
	for (j = 0; j < row; j++) {
		for (i = 0; i < row; i++) {
			*(S + j) += *(A + row * i + j) * *(A + row * i + j);
		}

		*(S + j) = sqrtf(*(S + j));
	}

	// Sort the singular values largest to smallest, and the right matrix accordingly
	for (p = 0; p < (row - 1); p++) {
		for (j = 0; j < row - p - 1; j++) {
			if (*(S + j) < *(S + j + 1)) {
				float tmp = *(S + j);

				*(S + j) = *(S + j + 1);
				*(S + j + 1) = tmp;

				// Rearrange columns of u and v accordingly
				for (i = 0; i < row; i++) {
					float tmp = *(V + row * i + j);

					*(V + row * i + j) = *(V + row * i + j + 1);
					*(V + row * i + j + 1) = tmp;

					tmp = *(A + row * i + j);
					*(A + row * i + j) = *(A + row * i + j + 1);
					*(A + row * i + j + 1) = tmp;
				}
			}
		}
	}

	// A is A*V, so in order to get U, we divide by S in each column
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			*(A + row * i + j) = *(A + row * i + j) / *(S + j);
		}
	}

	// Set U to A, since we have been making modifications to A
	memcpy(U, A, row * row * sizeof(float));
}

/*
 * GNU Octave code:
 *  >> A
	A =

	   3   4   5
	   2   5   6
	   5   6   7

	>> [u, s, v] = svd(A)
	u =

	  -0.47384   0.10757  -0.87402
	  -0.53340  -0.82478   0.18767
	  -0.70069   0.55512   0.44819

	s =

	Diagonal Matrix

	   14.91526          0          0
			  0    1.58310          0
			  0          0    0.16940

	v =

	  -0.401719   0.915134  -0.033930
	  -0.587752  -0.229240   0.775885
	  -0.702260  -0.331630  -0.629962

	>>
 */
