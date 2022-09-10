/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#pragma once

#include <stdint.h>

/**
 * \brief Recursive least square. We estimate A(q)y(t) = B(q) + C(q)e(t)
 * \param NP Number of poles
 * \param NZ Number of zeros
 * \param NZE Number of zeros in error
 * \param u Input vector
 * \param y Output vector
 * \param count Counter output
 * \param past_e Previous error output (y - sum(phi .* theta))
 * \param past_y Past y = -y
 * \param past_u Past u = u
 * \param theta [NP + NZ + NZE]
 * \param P [(NP + NZ + NZE)*(NP + NZ + NZE)]
 * \param phi [NP + NZ + NZE]
 * \param Pq Pq > 0
 * \param forgetting 0 < forgetting <= 1
 * \retval 0 Success
 * \retval -EINVAL Invalid arguments
 **/
int rls(unsigned int NP, unsigned int NZ, unsigned int NZE, float theta[], float u, float y,
	uint8_t *count, float *past_e, float *past_y, float *past_u, float phi[], float P[],
	float Pq, float forgetting);
/**
 * \brief Eigensystem Realization Algorithm.
 * \param A [ADIM*ADIM] // System matrix with dimension ADIM*ADIM
 * \param B [ADIM*io_row] // Input matrix with dimension ADIM*inputs_outputs
 * \param C [io_row*ADIM] // Output matrix with dimension inputs_outputs*ADMIN
 * \param u [m*n] // Input signal
 * \param y [m*n] // Output signal
 * \param io_row Rows in input and output signal
 * \param io_column Columns in input and output signal
 * \param row_a Rows in A
 * \retval 0 Success
 * \retval -EINVAL Invalid parameters
 **/
int okid_era(float *A, float *B, float *C, uint8_t row_a, const float *const y,
	     const float *const u, uint16_t io_row, uint16_t io_column);
/**
 * \brief Square Root Unscented Kalman Filter
 * \details For Parameter Estimation (A better version than regular UKF)
 * \param L Number of states, or sensors in practice.
 * \param beta used to incorporate prior knowledge of the distribution of x (for
 *    Gaussian distributions, beta = 2 is optimal)
 * \param alpha determines the spread of the sigma points around what and alpha
 *    is usually set to 0.01 <= alpha <= 1
 * \param Sw [L * L] Parameter estimate error covariance
 * \param lambda_rls Scalar factor 0 <= lambda_rls <= 1. A good number is close to 1 like 0.995
 * \param G G(float dw[L], float x[L], float w[L]) = Transition function with unknown parameters
 * \param x [L] State vector
 * \param Re [L * L] Measurement noise covariance matrix
 * \param what [L] Estimated parameter (our input)
 * \param d [L] Measurement parameter (our output)
 **/
int sqr_ukf_id(float d[], float what[], float Re[], float x[], void (*G)(float[], float[], float[]),
	       float lambda_rls, float Sw[], float alpha, float beta, uint8_t L);
