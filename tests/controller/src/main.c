/*
 ============================================================================
 Name        : Kalman.c
 Author      : Daniel M�rtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Kalman filter estimation example
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unity.h>
#include <control/controller.h>
#include <control/misc.h>

void test_kalman_filter(void)
{
#define ADIM 2
#define RDIM 2
#define YDIM 2

	// Create our matrices
	float A[ADIM * ADIM] = { 0.79499, 0.33654, -0.70673, 0.35749 };
	float B[ADIM * RDIM] = { 0.2050072, 0.0097622, 0.7067279, 0.0336537 };
	float C[YDIM * ADIM] = { 1.20000, 0.00000, 0.00000, 0.30000 };
	float K[ADIM * YDIM] = { 0.532591, -0.066108, -0.503803, 0.136580 };

	/*
	 * Print our state space matrix
	 */
	printf("System matrix: A\n");
	print(A, ADIM, ADIM);

	printf("Input matrix: B\n");
	print(B, ADIM, RDIM);

	printf("Output matrix: C\n");
	print(C, YDIM, ADIM);

	printf("Kalman gain matrix: K\n");
	print(K, ADIM, YDIM);

	// Create input u, output y and state vector x
	float u[RDIM] = { 2.2, 4.3 };
	float y[YDIM] = { 5.134, 0.131 };
	float x[ADIM] = { 2.32, 4.12 };

	// Call kalman function and estimate our future state at x(k+1)
	printf("Our current state vector is:\n");
	print(x, ADIM, 1);
	kalman(A, B, C, K, u, x, y, ADIM, YDIM, RDIM);
	printf("Our estimated state vector is:\n");
	print(x, ADIM, 1);
#undef RDIM
#undef YDIM
#undef ADIM
}

void test_control_lqi_regulator(void)
{
#define ADIM 2
#define RDIM 1
#define YDIM 1
#define ANTI_WINDUP 0

	printf("Control library LQI sample\n");

	// Model A, B, C, D
	float A[ADIM * ADIM] = { 0.89559, 0.37735, -0.37735, 0.51825 };

	float B[ADIM * RDIM] = { 0.20881, 0.75469 };

	float C[YDIM * ADIM] = { 1, 0 };

	// Kalman steady state matrix K
	float K[ADIM * YDIM] = { 0.58006, -0.22391 };

	// Control law L
	float L[RDIM * ADIM] = { 1.56766, 0.85103 };

	// Integral law Li
	float Li[RDIM] = { 0.50135 };

	// Initial states - Normally set to zero
	float x[ADIM] = { 0 };
	float xi[1] = { 0 };

	// Input signal, reference signal, output signal and state vector x and xi
	float qi = 0.1; // <-- Tune this in between 0->1, but qi =/= 1.
	float u[RDIM] = { 0 }; // <-- This is our input value we want to have
	float r[RDIM] = { 25 }; // <-- This is what we want to see.
	float y[YDIM] = { 0 }; // <-- This is our measurement. Set it.

	//Control LQI
	lqi(y, u, qi, r, L, Li, x, xi, ADIM, YDIM, RDIM, ANTI_WINDUP);

	// Estimate the state x
	kalman(A, B, C, K, u, x, y, ADIM, YDIM, RDIM);

	// Check the inputs
	printf("u = %f, y = %f\n", u[0], y[0]);
#undef RDIM
#undef YDIM
#undef ADIM
}

void test_mpi_controller(void)
{
#define ADIM 2
#define RDIM 1
#define YDIM 1
#define HORIZON 20
#define ITERATION_LIMIT 200

	// State space model that have integration behavior
	float A[ADIM * ADIM] = { 1.71653, 1.00000, -0.71653, 0.00000 };

	float B[ADIM * RDIM] = { 0.18699, 0.16734 };
	float C[YDIM * ADIM] = { 1, 0 };

	// Print our state space matrix
	printf("System matrix: A\n");
	print(A, ADIM, ADIM);

	printf("Input matrix: B\n");
	print(B, ADIM, RDIM);

	printf("Output matrix: C\n");
	print(C, YDIM, ADIM);

	float x[ADIM] = { 0, 0 };
	float u[RDIM] = { 0 };
	float r[YDIM] = { 12.5 };
	float K[ADIM] = { 0, 0 };
	float y[YDIM] = { 0 };
	float inputs[200];

	// Do Model Predictive Control where we selecting last u
	for (int i = 0; i < 200; i++) {
		mpc(A, B, C, x, u, r, ADIM, YDIM, RDIM, HORIZON, ITERATION_LIMIT, 1);
		kalman(A, B, C, K, u, x, y, ADIM, YDIM, RDIM); // Do only state update x = Ax + Bu
		inputs[i] = u[0];
	}

	printf("Complete inputs:\n");
	print(inputs, 200, 1);
#undef ADIM
#undef RDIM
#undef YDIM
#undef HORIZON
#undef ITERATION_LIMIT
}

void test_mrac_controller(void)
{
#define RDIM 1
#define LIMIT 1
#define LEARNING 0.001

	/* MRAC works as K1 and K2 integrates over time so y -> r when time -> inf
	 * r ----{K1}*(-LEARNING)---->+()----->G(s)-----------------------> y
	 *               			  -^            			  |
	 *               			   |            			  |
	 *                			   |              			  |
	 *                  		   |---*(LEARNING){K2}<--------
	 */

	float y[RDIM] = { 7 };
	float u[RDIM] = { 5 };
	float r[RDIM] = { 51 };
	float I1[RDIM] = { 3.1 };
	float I2[RDIM] = { 4.1 };

	// Compute now
	mrac(LIMIT, LEARNING, y, u, r, I1, I2, RDIM);

	// Show input signal
	printf("Our output signal\n");
	print(u, RDIM, 1);

#undef RDIM
#undef LIMIT
#undef LEARNING
}
