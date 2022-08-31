// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include <math.h>
#include <float.h>
#include <string.h>
#include <errno.h>
#include <control/linalg.h>

// Private functions
static void Householders_Reduction_to_Bidiagonal_Form(const float *const A, uint16_t nrows,
						      uint16_t ncols, float *U, float *V,
						      float *diagonal, float *superdiagonal);
static int Givens_Reduction_to_Diagonal_Form(uint16_t nrows, uint16_t ncols, float *U, float *V,
					     float *diagonal, float *superdiagonal);
static void Sort_by_Decreasing_Singular_Values(uint16_t nrows, uint16_t ncols,
					       float *singular_value, float *U, float *V);

int svd_golub_reinsch(const float *const A, uint16_t row, uint16_t column, float *U, float *S,
		      float *V)
{
	float dummy_array[column];

	Householders_Reduction_to_Bidiagonal_Form(A, row, column, U, V, S, dummy_array);

	if (Givens_Reduction_to_Diagonal_Form(row, column, U, V, S, dummy_array) < 0)
		return -ENOTSUP;

	Sort_by_Decreasing_Singular_Values(row, column, S, U, V);

	return 0;
}

static void Householders_Reduction_to_Bidiagonal_Form(const float *const Ain, uint16_t nrows,
						      uint16_t ncols, float *U, float *V,
						      float *diagonal, float *superdiagonal)
{
	int i, j, k, ip1;
	float s, s2, si, scale;
	float *pu, *pui, *pv, *pvi;
	float half_norm_squared;

	float A[nrows * ncols];

	memcpy(A, Ain, sizeof(A));
	memcpy(U, Ain, sizeof(float) * nrows * ncols);

	diagonal[0] = 0.0;
	s = 0.0;
	scale = 0.0;
	for (i = 0, pui = U, ip1 = 1; i < ncols; pui += ncols, i++, ip1++) {
		superdiagonal[i] = scale * s;
		//
		//       Perform Householder transform on columns.
		//
		//       Calculate the normed squared of the i-th column vector starting at
		//       row i.
		//
		for (j = i, pu = pui, scale = 0.0; j < nrows; j++, pu += ncols)
			scale += fabsf(*(pu + i));

		if (scale > 0.0) {
			for (j = i, pu = pui, s2 = 0.0; j < nrows; j++, pu += ncols) {
				*(pu + i) /= scale;
				s2 += *(pu + i) * *(pu + i);
			}
			//
			//
			//       Chose sign of s which maximizes the norm
			//
			s = (*(pui + i) < 0.0) ? sqrtf(s2) : -sqrtf(s2);
			//
			//       Calculate -2/u'u
			//
			half_norm_squared = *(pui + i) * s - s2;
			//
			//       Transform remaining columns by the Householder transform.
			//
			*(pui + i) -= s;

			for (j = ip1; j < ncols; j++) {
				for (k = i, si = 0.0, pu = pui; k < nrows; k++, pu += ncols)
					si += *(pu + i) * *(pu + j);
				si /= half_norm_squared;
				for (k = i, pu = pui; k < nrows; k++, pu += ncols) {
					*(pu + j) += si * *(pu + i);
				}
			}
		}
		for (j = i, pu = pui; j < nrows; j++, pu += ncols)
			*(pu + i) *= scale;
		diagonal[i] = s * scale;
		//
		//       Perform Householder transform on rows.
		//
		//       Calculate the normed squared of the i-th row vector starting at
		//       column i.
		//
		s = 0.0;
		scale = 0.0;
		if (i >= nrows || i == (ncols - 1))
			continue;
		for (j = ip1; j < ncols; j++)
			scale += fabsf(*(pui + j));
		if (scale > 0.0) {
			for (j = ip1, s2 = 0.0; j < ncols; j++) {
				*(pui + j) /= scale;
				s2 += *(pui + j) * *(pui + j);
			}
			s = (*(pui + ip1) < 0.0) ? sqrtf(s2) : -sqrtf(s2);
			//
			//       Calculate -2/u'u
			//
			half_norm_squared = *(pui + ip1) * s - s2;
			//
			//       Transform the rows by the Householder transform.
			//
			*(pui + ip1) -= s;
			for (k = ip1; k < ncols; k++)
				superdiagonal[k] = *(pui + k) / half_norm_squared;
			if (i < (nrows - 1)) {
				for (j = ip1, pu = pui + ncols; j < nrows; j++, pu += ncols) {
					for (k = ip1, si = 0.0; k < ncols; k++)
						si += *(pui + k) * *(pu + k);
					for (k = ip1; k < ncols; k++) {
						*(pu + k) += si * superdiagonal[k];
					}
				}
			}
			for (k = ip1; k < ncols; k++)
				*(pui + k) *= scale;
		}
	}

	// Update V
	pui = U + ncols * (ncols - 2);
	pvi = V + ncols * (ncols - 1);
	*(pvi + ncols - 1) = 1.0;
	s = superdiagonal[ncols - 1];
	pvi -= ncols;
	for (i = ncols - 2, ip1 = ncols - 1; i >= 0; i--, pui -= ncols, pvi -= ncols, ip1--) {
		if (s != 0.0) {
			pv = pvi + ncols;
			for (j = ip1; j < ncols; j++, pv += ncols)
				*(pv + i) = (*(pui + j) / *(pui + ip1)) / s;
			for (j = ip1; j < ncols; j++) {
				si = 0.0;
				for (k = ip1, pv = pvi + ncols; k < ncols; k++, pv += ncols)
					si += *(pui + k) * *(pv + j);
				for (k = ip1, pv = pvi + ncols; k < ncols; k++, pv += ncols)
					*(pv + j) += si * *(pv + i);
			}
		}
		pv = pvi + ncols;
		for (j = ip1; j < ncols; j++, pv += ncols) {
			*(pvi + j) = 0.0;
			*(pv + i) = 0.0;
		}
		*(pvi + i) = 1.0;
		s = superdiagonal[i];
	}

	// Update U

	pui = U + ncols * (ncols - 1);
	for (i = ncols - 1, ip1 = ncols; i >= 0; ip1 = i, i--, pui -= ncols) {
		s = diagonal[i];
		for (j = ip1; j < ncols; j++)
			*(pui + j) = 0.0;
		if (s != 0.0) {
			for (j = ip1; j < ncols; j++) {
				si = 0.0;
				pu = pui + ncols;
				for (k = ip1; k < nrows; k++, pu += ncols)
					si += *(pu + i) * *(pu + j);
				si = (si / *(pui + i)) / s;
				for (k = i, pu = pui; k < nrows; k++, pu += ncols)
					*(pu + j) += si * *(pu + i);
			}
			for (j = i, pu = pui; j < nrows; j++, pu += ncols) {
				*(pu + i) /= s;
			}
		} else
			for (j = i, pu = pui; j < nrows; j++, pu += ncols)
				*(pu + i) = 0.0;
		*(pui + i) += 1.0;
	}
}
//
static int Givens_Reduction_to_Diagonal_Form(uint16_t nrows, uint16_t ncols, float *U, float *V,
					     float *diagonal, float *superdiagonal)
{
	float epsilon;
	float c, s;
	float f, g, h;
	float x, y, z;
	float *pu, *pv;
	int i, j, k, m;
	int rotation_test;
	int iteration_count;

	for (i = 0, x = 0.0; i < ncols; i++) {
		y = fabsf(diagonal[i]) + fabsf(superdiagonal[i]);
		if (x < y)
			x = y;
	}
	epsilon = x * FLT_EPSILON;
	for (k = ncols - 1; k >= 0; k--) {
		iteration_count = 0;
		while (1) {
			rotation_test = 1;
			for (m = k; m >= 0; m--) {
				if (fabsf(superdiagonal[m]) <= epsilon) {
					rotation_test = 0;
					break;
				}
				if (fabsf(diagonal[m - 1]) <= epsilon)
					break;
			}
			if (rotation_test) {
				c = 0.0;
				s = 1.0;
				for (i = m; i <= k; i++) {
					f = s * superdiagonal[i];
					superdiagonal[i] *= c;
					if (fabsf(f) <= epsilon)
						break;
					g = diagonal[i];
					h = sqrtf(f * f + g * g);
					diagonal[i] = h;
					c = g / h;
					s = -f / h;
					for (j = 0, pu = U; j < nrows; j++, pu += ncols) {
						y = *(pu + m - 1);
						z = *(pu + i);
						*(pu + m - 1) = y * c + z * s;
						*(pu + i) = -y * s + z * c;
					}
				}
			}
			z = diagonal[k];
			if (m == k) {
				if (z < 0.0) {
					diagonal[k] = -z;
					for (j = 0, pv = V; j < ncols; j++, pv += ncols)
						*(pv + k) = -*(pv + k);
				}
				break;
			}
			if (iteration_count >= MAX_ITERATION_COUNT_SVD)
				return -1;
			iteration_count++;
			x = diagonal[m];
			y = diagonal[k - 1];
			g = superdiagonal[k - 1];
			h = superdiagonal[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
			g = sqrtf(f * f + 1.0);
			if (f < 0.0)
				g = -g;
			f = ((x - z) * (x + z) + h * (y / (f + g) - h)) / x;
			// Next QR Transformtion
			c = 1.0;
			s = 1.0;
			for (i = m + 1; i <= k; i++) {
				g = superdiagonal[i];
				y = diagonal[i];
				h = s * g;
				g *= c;
				z = sqrtf(f * f + h * h);
				superdiagonal[i - 1] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = -x * s + g * c;
				h = y * s;
				y *= c;
				for (j = 0, pv = V; j < ncols; j++, pv += ncols) {
					x = *(pv + i - 1);
					z = *(pv + i);
					*(pv + i - 1) = x * c + z * s;
					*(pv + i) = -x * s + z * c;
				}
				z = sqrtf(f * f + h * h);
				diagonal[i - 1] = z;
				if (z != 0.0) {
					c = f / z;
					s = h / z;
				}
				f = c * g + s * y;
				x = -s * g + c * y;
				for (j = 0, pu = U; j < nrows; j++, pu += ncols) {
					y = *(pu + i - 1);
					z = *(pu + i);
					*(pu + i - 1) = c * y + s * z;
					*(pu + i) = -s * y + c * z;
				}
			}
			superdiagonal[m] = 0.0;
			superdiagonal[k] = f;
			diagonal[k] = x;
		}
	}
	return 0;
}

static void Sort_by_Decreasing_Singular_Values(uint16_t nrows, uint16_t ncols,
					       float *singular_values, float *U, float *V)
{
	int i, j, max_index;
	float temp;
	float *p1, *p2;

	for (i = 0; i < ncols - 1; i++) {
		max_index = i;
		for (j = i + 1; j < ncols; j++)
			if (singular_values[j] > singular_values[max_index])
				max_index = j;
		if (max_index == i)
			continue;
		temp = singular_values[i];
		singular_values[i] = singular_values[max_index];
		singular_values[max_index] = temp;
		p1 = U + max_index;
		p2 = U + i;
		for (j = 0; j < nrows; j++, p1 += ncols, p2 += ncols) {
			temp = *p1;
			*p1 = *p2;
			*p2 = temp;
		}
		p1 = V + max_index;
		p2 = V + i;
		for (j = 0; j < ncols; j++, p1 += ncols, p2 += ncols) {
			temp = *p1;
			*p1 = *p2;
			*p2 = temp;
		}
	}
}

/*
 * GNU Octave code:
 *  >> A = [3 4 5; 3 5 6; 6 7 8; 3 5 6];
	>> [u, s, v] = svd(A)
	u =

	  -3.8485e-01   4.1233e-02   9.2206e-01  -5.6889e-16
	  -4.5421e-01   4.9880e-01  -2.1189e-01  -7.0711e-01
	  -6.6278e-01  -7.0760e-01  -2.4499e-01   1.8751e-16
	  -4.5421e-01   4.9880e-01  -2.1189e-01   7.0711e-01

	s =

	Diagonal Matrix

	   18.36763          0          0
			  0    1.26360          0
			  0          0    0.18280
			  0          0          0

	v =

	  -0.42774  -0.89359   0.13617
	  -0.58369   0.15803  -0.79645
	  -0.69018   0.42015   0.58917

	>>
 */
