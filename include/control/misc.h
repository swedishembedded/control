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

void cat(uint8_t dim, float A[], float B[], float C[], uint16_t row_a, uint16_t column_a,
	 uint16_t row_b, uint16_t column_b, uint16_t row_c, uint16_t column_c);
float saturation(float input, float lower_limit, float upper_limit);
void cut(float A[], uint16_t row, uint16_t column, float B[], uint16_t start_row, uint16_t stop_row,
	 uint16_t start_column, uint16_t stop_column);
void insert(float A[], float B[], uint16_t row_a, uint16_t column_a, uint16_t column_b,
	    uint16_t startRow_b, uint16_t startColumn_b);
void print(float A[], uint16_t row, uint16_t column);
float sign(float number);
void randn(float x[], uint16_t length, float mu, float sigma);
float mean(float x[], uint16_t length);
float stddev(float x[], uint16_t length);
float vmax(float a, float b);
float vmin(float a, float b);
