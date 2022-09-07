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
 * \brief Observer kalman filter identification.
 * \details
 * This is the basic version, e.g it won't give you the kalman gain K matrix.
 * If you need fully version, then look for MataveID at GitHub
 * First collect your inputs u and outputs y and create impulse response g,
 * called Markov parameters.  Then you must use era.c algorithm to convert
 * impulse response g into a linear state space model.  Data length need to be
 * the same as the column length n!
 * \param row Number of rows in input
 * \param column Number of columns in input
 * \param u [m*n]
 * \param y [m*n]
 * \param g [m*n] Markov parameters
 **/
void okid(float u[], float y[], float g[], uint16_t row, uint16_t column);
/**
 * \brief Eigensystem Realization Algorithm.
 * \param u [m*n] // Input signal
 * \param y [m*n] // Output signal
 * \param row Rows in input signal
 * \param column Columns in input signal
 * \param row_a Rows in A
 * \param A [ADIM*ADIM] // System matrix with dimension ADIM*ADIM
 * \param B [ADIM*inputs_outputs] // Input matrix with dimension ADIM*inputs_outputs
 * \param C [inputs_outputs*ADIM] // Output matrix with dimension inputs_outputs*ADMIN
 * \param inputs_outputs number of inputs and outputs
 * \retval 0 Success
 * \retval -EINVAL Invalid parameters
 **/
int era(float u[], float y[], uint16_t row, uint16_t column, float A[], float B[], float C[],
	uint8_t row_a, uint8_t inputs_outputs);
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
