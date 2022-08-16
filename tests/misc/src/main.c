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
#include <control/misc.h>

void test_cat(void)
{
	float A[2 * 3] = { 1, 2, 3, 4, 5, 6 };
	float B[2 * 4] = { 7, 8, 9, 6, 1, 2, 3, 5 };
	float C[2 * 7] = { 0 };

	cat(2, A, B, C, 2, 3, 2, 4, 2, 7); /* do C = [A B] */
	print(C, 2, 7);
}

void test_cut(void)
{
	// Matrix A
	float A[5 * 5] = { 0.87508, 0.48607, 0.30560, 0.32509, 0.23096, 0.12308, 0.84311,
			   0.42221, 0.20273, 0.87377, 0.42986, 0.11245, 0.40494, 0.27304,
			   0.59772, 0.85124, 0.99245, 0.56873, 0.19438, 0.11308, 0.97190,
			   0.22475, 0.24501, 0.85403, 0.54691 };

	// Matrix B
	float B[2 * 3];

	// Cut a matrix
	cut(A, 5, 5, B, 3, 4, 2, 4); // We want to B = A(4:5, 3:5)
	// Print B
	printf("B\n");
	print(B, 2, 3);
}

void test_insert_cut(void)
{
	// Matrix A
	float A[5 * 5] = { 0.87508, 0.48607, 0.30560, 0.32509, 0.23096, 0.12308, 0.84311,
			   0.42221, 0.20273, 0.87377, 0.42986, 0.11245, 0.40494, 0.27304,
			   0.59772, 0.85124, 0.99245, 0.56873, 0.19438, 0.11308, 0.97190,
			   0.22475, 0.24501, 0.85403, 0.54691 };

	// Matrix B
	float B[2 * 3] = { 1, 1, 1, 2, 2, 2 };

	// Insert B into A at a specific row and column
	insert(B, A, 2, 3, 5, 3, 2);
	cut(A, 5, 5, B, 3, 4, 2, 4); // We want to B = A(4:5, 3:5)

	// Print A
	printf("A\n");
	print(A, 5, 5);
}

void test_randn(void)
{
	float x[1000];
	randn(x, 1000, 10, 10);
	float Mean = 0;
	for (int i = 0; i < 1000; i++)
		Mean += x[i];
	Mean = Mean / 1000;
	printf("Mean = %f\n", Mean);
}
