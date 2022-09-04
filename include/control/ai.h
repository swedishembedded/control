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
 * \brief A* algorithm for path finding
 * \details
 * It find the optimal(at least try too) path from your source to your destination.
 * See working example how to use.
 * \param map Input map
 * \param path_x Output path x coordinates
 * \param path_y Output path y coordinates
 * \param x_start Starting x position
 * \param y_start Starting y position
 * \param x_stop End x position
 * \param y_stop End y position
 * \param height Height of the map
 * \param width Width of the map
 * \param norm_mode 1 or 2 (L1 or L2 norm)
 * \param steps Output variable which will contain number of steps taken
 **/
void a_star(const int *const map, int path_x[], int path_y[], int x_start, int y_start, int x_stop,
	    int y_stop, int height, int width, uint8_t norm_mode, int *steps);
/**
 * \brief Check if a point is inside a 2D polygon
 * \param x X coordinate
 * \param y Y coordinate
 * \param px X coordinates of the polygon
 * \param py Y coordinates of the polygon
 * \param p number of vertices
 * \retval 1 if inside
 * \retval 0 if outside
 **/
uint8_t inpolygon(float x, float y, const float *const px, const float *const py, uint8_t p);
