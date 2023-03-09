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
 * \brief Filter an array.
 * \details
 * Very simple method, but powerful
 * \param y_out Filtered Y (can be same as y)
 * \param y Vector [l]
 * \param t Vector [l]
 * \param l Length of both vectors
 * \param K gain K > 0, but small number
 **/
void filtfilt(float *y_out, const float *const y, const float *const t, uint16_t l, float K);
void mcs_collect(float P[], uint16_t column_p, float x[], uint8_t row_x, float index_factor);
void mcs_estimate(float P[], uint16_t column_p, float x[], uint8_t row_x);
void mcs_clean(float P[], uint16_t column_p, uint8_t row_x);
/**
 * \brief Square Root Unscented Kalman Filter
 * \details For State Estimation (A better version than regular UKF)
 * \param L Number of states, or sensors in practice.
 * \param beta Used to incorporate prior knowledge of the distribution of x (for
 *    Gaussian distributions, beta = 2 is optimal)
 * \param alpha Determines the spread of the sigma points around xhat and alpha
 *    is usually set to 0.01 <= alpha <= 1
 * \param S [L * L] State estimate error covariance
 * \param F F(float dx[L], float x[L], float u[L]) = Transition function
 * \param xhat [L] State vector
 * \param u [L] Input signal
 * \param Rv [L * L] Process noise covariance matrix
 * \param Rn [L * L] Measurement noise covariance matrix
 * \param xhat [L] Estimated state (our input)
 * \param y [L] Measurement state (our output)
 * \retval 0 Success
 * \retval -EINVAL Invalid parameters
 **/
int sqr_ukf(float y[], float xhat[], float Rn[], float Rv[], float u[],
	    void (*F)(float[], float[], float[]), float S[], float alpha, float beta, uint8_t L);
