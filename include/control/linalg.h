/* SPDX-License-Identifier: MIT */
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_ITERATION_COUNT_SVD 30 // Maximum number of iterations for svd_jacobi_one_sided.c

uint8_t inv(float *A, uint16_t row);
void linsolve_upper_triangular(float *A, float *x, float *b, uint16_t column);
void tran(float A[], uint16_t row, uint16_t column);
void mul(float A[], float B[], float C[], uint16_t row_a, uint16_t column_a, uint16_t column_b);
void svd_jacobi_one_sided(float A[], uint16_t row, uint8_t max_iterations, float U[], float S[],
			  float V[]);
void dlyap(float A[], float P[], float Q[], uint16_t row);
uint8_t svd_golub_reinsch(float A[], uint16_t row, uint16_t column, float U[], float S[],
			  float V[]);
uint8_t qr(float A[], float Q[], float R[], uint16_t row_a, uint16_t column_a, bool only_compute_R);
void linsolve_qr(float A[], float x[], float b[], uint16_t row, uint16_t column);
void linsolve_lower_triangular(float A[], float x[], float b[], uint16_t row);
uint8_t lup(float A[], float LU[], uint8_t P[], uint16_t row);
float det(float A[], uint16_t row);
uint8_t linsolve_lup(float A[], float x[], float b[], uint16_t row);
void chol(float A[], float L[], uint16_t row);
void cholupdate(float L[], float x[], uint16_t row, bool rank_one_update);
void linsolve_chol(float A[], float x[], float b[], uint16_t row);
void pinv(float A[], uint16_t row, uint16_t column);
void hankel(float V[], float H[], uint16_t row_v, uint16_t column_v, uint16_t row_h,
	    uint16_t column_h, uint16_t shift);
void balance(float A[], uint16_t row);
void eig(float A[], float wr[], float wi[], uint16_t row);
void eig_sym(float A[], uint16_t row, float d[]);
void sum(float A[], uint16_t row, uint16_t column, uint8_t l);
float norm(float A[], uint16_t row, uint16_t column, uint8_t l);
void expm(float A[], uint16_t row);
void nonlinsolve(void (*nonlinear_equation_system)(float[], float[], float[]), float b[], float x[],
		 uint8_t elements, float alpha, float max_value, float min_value,
		 bool random_guess_active);
void linsolve_gauss(float *A, float *x, float *b, uint16_t row, uint16_t column, float alpha);
