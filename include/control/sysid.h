/* SPDX-License-Identifier: MIT */
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#pragma once
#include <stdint.h>

void rls(uint8_t NP, uint8_t NZ, uint8_t NZE, float theta[], float u, float y, uint8_t *count,
	 float *past_e, float *past_y, float *past_u, float phi[], float P[], float Pq,
	 float forgetting);
void okid(float u[], float y[], float g[], uint16_t row, uint16_t column);
void era(float u[], float y[], uint16_t row, uint16_t column, float A[], float B[], float C[],
	 uint8_t row_a, uint8_t inputs_outputs);
void sqr_ukf_id(float d[], float what[], float Re[], float x[],
		void (*G)(float[], float[], float[]), float lambda_rls, float Sw[], float alpha,
		float beta, uint8_t L);
