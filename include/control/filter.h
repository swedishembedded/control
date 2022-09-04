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
void sqr_ukf(float y[], float xhat[], float Rn[], float Rv[], float u[],
	     void (*F)(float[], float[], float[]), float S[], float alpha, float beta, uint8_t L);
