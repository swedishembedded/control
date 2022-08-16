/*
 ============================================================================
 Name        : Astar.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : MIT
 Description : A* algorithm - Find the shortest path
 ============================================================================
 */

#include <unity.h>
#include <stdio.h>
#include <control/ai.h>

#define height_map 15
#define width_map 10

// This function is here only for illustration. Not needed
static void print_int(int A[], int row, int column)
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
static void show_path(int map[], int path_x[], int path_y[], int height, int width)
{
	for (int k = 0; k < height * width; k++) {
		int x = path_x[k];
		int y = path_y[k];
		if (x != -1 && y != -1)
			map[y * width + x] = -100 - k;
	}
	print_int(map, height, width);
}

void test_astar_search(void)
{
	// Beginning coordinates
	int x_start = 8;
	int y_start = 13;

	// End coordinates
	int x_stop = 6;
	int y_stop = 2;

	// Path - Our goal is to find them
	int path_x[height_map * width_map];
	int path_y[height_map * width_map];

	// Norm modes
	int norm1 = 1; // L1-Norm
	int norm2 = 2; // L2-Norm

	// Steps
	int steps1 = 0;
	int steps2 = 0;

	// Map size
	int map[height_map * width_map] = {
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
	int map[height_map*width_map] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
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
	print_int(map, height_map, width_map);

	// Compute the "shortest" path
	printf("Compute the coordinates\n");

	// First compute with L1-Norm and check the steps
	Astar(map, path_x, path_y, x_start, y_start, x_stop, y_stop, height_map, width_map, norm1,
	      &steps1);
	// Then compute with L2-norm and check the steps
	Astar(map, path_x, path_y, x_start, y_start, x_stop, y_stop, height_map, width_map, norm2,
	      &steps2);
	// Check the steps now - Which of the norms results less steps
	if (steps2 > steps1) {
		Astar(map, path_x, path_y, x_start, y_start, x_stop, y_stop, height_map, width_map,
		      norm1, &steps1); // Get the path again
		printf("Shortest step is = %d\n", steps1);
	} else {
		printf("Shortest step is = %d\n", steps2);
	}

	// Show the path
	printf("\nComputed map\n");
	show_path(map, path_x, path_y, height_map, width_map);

	// Show the path
	for (int i = 0; i < height_map * width_map; i++)
		printf("x = %d, y = %d\n", path_x[i], path_y[i]);
}

void test_inpoly_inside_polygon(void)
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
	TEST_ASSERT_TRUE(is_inside);
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
