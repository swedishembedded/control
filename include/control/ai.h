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

void Astar(int map[], int path_x[], int path_y[], int x_start, int y_start, int x_stop, int y_stop,
	   int height, int width, uint8_t norm_mode, int *steps);
uint8_t inpolygon(float x, float y, float px[], float py[], uint8_t p);
