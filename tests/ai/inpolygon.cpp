/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Copyright 2017 Daniel Mårtensson
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/dynamics.h"
#include "control/ai.h"
};

TEST(Main, inpolygon)
{
	/* Create the polygon coordinates */
	float px[12] = { 47.3364, 59.7788, 65.0323, 62.5438, 51.4839, 38.7650,
			 36.5530, 36.5530, 37.1060, 40.1475, 46.5069, 47.3364 };
	float py[12] = { -3.2484, 6.8953, 22.4731, 32.2546,  39.5001,  33.3414,
			 18.8504, 5.4462, -3.9730, -11.9430, -12.3053, -3.2484 };
	uint8_t p = 12;

	/* Check if this coordinate is inside the polygon */
	float x = 62.035;
	float y = 22.473;

	/* Perform algorithm */
	uint8_t is_inside = inpolygon(x, y, px, py, p);
	ASSERT_TRUE(is_inside);
}

/* Gnu Octave code:
 *
 * 	>> XV = [47.3364,   59.7788,   65.0323,   62.5438,   51.4839,   38.7650,   36.5530,   36.5530,   37.1060,   40.1475,   46.5069,   47.3364];
	>> YV = [-3.2484,    6.8953,   22.4731,   32.2546,   39.5001,   33.3414,   18.8504,    5.4462,   -3.9730,  -11.9430,  -12.3053,  -3.2484];
	>> inpolygon (65.034, 22.473, XV, YV)
	ans = 0
	>> inpolygon (65.032, 22.473, XV, YV)
	ans = 1
	>>
 */
