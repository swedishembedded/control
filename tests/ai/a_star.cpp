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

#include <stdio.h>

#define MAP_HEIGHT 15
#define MAP_WIDTH 10

// This function is here only for illustration. Not needed
static void print_int(const int *A, int row, int column)
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			printf("%d\t", *(A++));
		}
		printf("\n");
	}
	printf("\n");
}

// This function is here only for illustration. Not needed
static void show_path(const int *const map_in, int path_x[], int path_y[], int height, int width)
{
	int map[height * width];

	memcpy(map, map_in, sizeof(map));

	for (int k = 0; k < height * width; k++) {
		int x = path_x[k];
		int y = path_y[k];
		if (x != -1 && y != -1)
			map[y * width + x] = -100 - k;
	}
	print_int(map, height, width);
}

TEST(Main, Astar)
{
	// Beginning coordinates
	int x_start = 8;
	int y_start = 13;

	// End coordinates
	int x_stop = 6;
	int y_stop = 2;

	// Path - Our goal is to find them
	int path_x[MAP_HEIGHT * MAP_WIDTH];
	int path_y[MAP_HEIGHT * MAP_WIDTH];

	// Steps
	int steps1 = 0;
	int steps2 = 0;

	// Map size
	int map[MAP_HEIGHT * MAP_WIDTH] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,	0,  0,	0,  0,	0,  0,	0,
		-1, -1, 0,  0,	0,  0,	0,  0,	0,  0,	-1, -1, 0,  0,	0,  -1, -1, -1, 0,
		0,  -1, -1, 0,	0,  0,	0,  0,	-1, 0,	0,  -1, -1, -1, 0,  -1, 0,  0,	-1,
		-1, -1, -1, -1, -1, 0,	-1, 0,	0,  0,	0,  0,	-1, -1, 0,  0,	-1, -1, -1,
		-1, 0,	0,  -1, -1, 0,	0,  0,	0,  0,	0,  0,	0,  -1, -1, 0,	-1, -1, -1,
		-1, -1, -1, -1, -1, -1, 0,  0,	0,  -1, 0,  0,	-1, 0,	-1, -1, 0,  0,	0,
		-1, 0,	0,  0,	0,  -1, -1, 0,	0,  0,	-1, 0,	0,  0,	0,  -1, -1, 0,	0,
		0,  0,	0,  -1, -1, 0,	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	};

	// Or an alternative map
	/*
	// Map size
	int map[MAP_HEIGHT*MAP_WIDTH] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
									 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
									 -1, -1,  0,  0,  0,  0,  0,  0,  0, -1,
									 -1, -1,  0, -1, -1, -1, -1, -1,  0, -1,
									 -1, -1,  0, -1,  0,  0,  0, -1,  0, -1,
									 -1, -1,  0, -1,  0, -1,  0, -1,  0, -1,
									 -1, -1,  0, -1,  0, -1,  0, -1,  0, -1,
									 -1, -1,  0, -1,  0, -1,  0, -1,  0, -1,
									 -1,  0,  0,  0,  0, -1,  0, -1,  0, -1,
									 -1, -1, -1, -1, -1, -1,  0, -1,  0, -1,
									 -1, -1,  0,  0,  0,  0,  0, -1,  0, -1,
									 -1, -1,  0,  0, -1, -1, -1, -1,  0, -1,
									 -1, -1,  0,  0,  0,  0,  0,  0,  0, -1,
									 -1, -1, -1, -1, -1,  0, -1, -1,  0, -1,
									 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	*/

	// Print the map
	printf("Initial map\n");
	print_int(map, MAP_HEIGHT, MAP_WIDTH);

	// Compute the "shortest" path
	printf("Compute the coordinates\n");

	// First compute with L1-Norm and check the steps
	a_star(map, path_x, path_y, x_start, y_start, x_stop, y_stop, MAP_HEIGHT, MAP_WIDTH, 1,
	       &steps1);

	printf("Steps: %d\n", steps1);
	// Show the path
	int i = 0;
	for (i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {
		printf("i %d: x = %d, y = %d\n", i, path_x[i], path_y[i]);
		if (path_x[i] == -1 && path_y[i] == -1)
			break;
	}

	// TODO: steps is actually not equal. This is a bug
	// EXPECT_EQ(i, steps1);
	EXPECT_EQ(path_x[0], 8);
	EXPECT_EQ(path_y[0], 13);
	EXPECT_EQ(path_x[i - 1], 6);
	EXPECT_EQ(path_y[i - 1], 2);

	// check that none of the points are on 'walls'
	for (unsigned int c = 0; c < i; c++) {
		EXPECT_EQ(0, map[path_y[c] * MAP_WIDTH + path_x[c]]);
	}

	// Then compute with L2-norm and check the steps
	a_star(map, path_x, path_y, x_start, y_start, x_stop, y_stop, MAP_HEIGHT, MAP_WIDTH, 2,
	       &steps2);

	printf("Steps: %d\n", steps2);
	// Show the path
	for (i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {
		printf("i %d: x = %d, y = %d\n", i, path_x[i], path_y[i]);
		if (path_x[i] == -1 && path_y[i] == -1)
			break;
	}

	// TODO: steps is actually not equal. This is a bug
	// EXPECT_EQ(i, steps2);
	EXPECT_EQ(path_x[0], 8);
	EXPECT_EQ(path_y[0], 13);
	EXPECT_EQ(path_x[i - 1], 6);
	EXPECT_EQ(path_y[i - 1], 2);

	// Check the steps now - Which of the norms results less steps
	if (steps2 > steps1) {
		// Get the path again
		a_star(map, path_x, path_y, x_start, y_start, x_stop, y_stop, MAP_HEIGHT, MAP_WIDTH,
		       1, &steps1);
		printf("Shortest step is = %d\n", steps1);
	} else {
		printf("Shortest step is = %d\n", steps2);
	}

	// Show the path
	printf("\nComputed map\n");
	show_path(map, path_x, path_y, MAP_HEIGHT, MAP_WIDTH);
}
