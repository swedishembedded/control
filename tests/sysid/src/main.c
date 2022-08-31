/* SPDX-License-Identifier: MIT */
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
#include <stdbool.h>
#include <control/sysid.h>
#include <control/controller.h>
#include <control/misc.h>

void test_era(void)
{
	float y[2 * 200] = {
		// This is the first signal
		0.00000, 0.54317, 1.13607, 1.74498, 2.34910, 2.93597, 3.49846, 4.03284, 4.53752,
		5.01223, 5.45753, 5.87447, 6.26433, 6.62856, 6.96862, 7.28597, 7.58206, 7.85824,
		8.11581, 8.35601, 8.57999, 8.78883, 8.98356, 9.16512, 9.33440, 9.49222, 9.63937,
		9.77656, 9.90447, 10.02373, 10.13491, 10.23857, 10.33521, 10.42532, 10.50932,
		10.58765, 10.66067, 10.72875, 10.79222, 10.85140, 10.90657, 10.95801, 11.00597,
		11.05068, 11.09237, 11.13123, 11.16747, 11.20125, 11.23275, 11.26211, 11.28949,
		11.31501, 11.33881, 11.36100, 11.38168, 11.40097, 11.41895, 11.43572, 11.45134,
		11.46592, 11.47950, 11.49217, 11.50398, 11.51499, 11.52525, 11.53482, 11.54374,
		11.55206, 11.55982, 11.56705, 11.57379, 11.58008, 11.58594, 11.59140, 11.59649,
		11.60124, 11.60567, 11.60980, 11.61365, 11.61723, 11.62058, 11.62370, 11.62661,
		11.62932, 11.63184, 11.63420, 11.63640, 11.63845, 11.64036, 11.64214, 11.64380,
		11.64534, 11.64679, 11.64813, 11.64939, 11.65056, 11.65165, 11.65266, 11.65361,
		11.65449, 11.65532, 11.33018, 10.97406, 10.60709, 10.24171, 9.88543, 9.54254,
		9.21535, 8.90487, 8.61131, 8.33441, 8.07361, 7.82818, 7.59732, 7.38020, 7.17600,
		6.98390, 6.80312, 6.63294, 6.47264, 6.32157, 6.17912, 6.04471, 5.91779, 5.79786,
		5.68445, 5.57712, 5.47545, 5.37907, 5.28762, 5.20076, 5.11818, 5.03960, 4.96474,
		4.89335, 4.82520, 4.76006, 4.69774, 4.63804, 4.58078, 4.52581, 4.47296, 4.42209,
		4.37307, 4.32577, 4.28007, 4.23588, 4.19308, 4.15158, 4.11130, 4.07214, 4.03404,
		3.99693, 3.96073, 3.92538, 3.89084, 3.85703, 3.82392, 3.79145, 3.75959, 3.72829,
		3.69751, 3.66722, 3.63738, 3.60797, 3.57895, 3.55030, 3.52200, 3.49402, 3.46633,
		3.43892, 3.41178, 3.38487, 3.35819, 3.33173, 3.30545, 3.27936, 3.25344, 3.22768,
		3.20207, 3.17660, 3.15126, 3.12603, 3.10092, 3.07591, 3.05100, 3.02618, 3.00145,
		2.97679, 2.95221, 2.92769, 2.90324, 2.87885, 2.85452, 2.83024, 2.80600, 2.78181,
		2.75766, 2.73355, 2.70948,
		// This is the second signal
		0.00000, 0.72166, 1.02233, 1.06116, 0.94064, 0.72659, 0.46102, 0.17061, -0.12794,
		-0.42432, -0.71234, -0.98847, -1.25084, -1.49865, -1.73176, -1.95045, -2.15521,
		-2.34668, -2.52556, -2.69257, -2.84844, -2.99386, -3.12950, -3.25601, -3.37398,
		-3.48399, -3.58656, -3.68220, -3.77137, -3.85451, -3.93203, -4.00430, -4.07168,
		-4.13450, -4.19307, -4.24768, -4.29859, -4.34606, -4.39032, -4.43158, -4.47004,
		-4.50591, -4.53934, -4.57052, -4.59958, -4.62668, -4.65195, -4.67550, -4.69746,
		-4.71793, -4.73702, -4.75482, -4.77141, -4.78688, -4.80130, -4.81475, -4.82729,
		-4.83898, -4.84987, -4.86003, -4.86950, -4.87834, -4.88657, -4.89425, -4.90140,
		-4.90807, -4.91430, -4.92010, -4.92550, -4.93054, -4.93524, -4.93963, -4.94371,
		-4.94752, -4.95107, -4.95438, -4.95747, -4.96035, -4.96303, -4.96553, -4.96787,
		-4.97004, -4.97207, -4.97396, -4.97572, -4.97736, -4.97890, -4.98032, -4.98166,
		-4.98290, -4.98405, -4.98513, -4.98614, -4.98708, -4.98795, -4.98877, -4.98953,
		-4.99024, -4.99090, -4.99151, -4.99209, -5.42562, -5.60798, -5.63381, -5.56407,
		-5.43795, -5.28045, -5.10749, -4.92903, -4.75125, -4.57786, -4.41101, -4.25184,
		-4.10086, -3.95818, -3.82367, -3.69706, -3.57800, -3.46609, -3.36093, -3.26212,
		-3.16924, -3.08193, -2.99982, -2.92257, -2.84985, -2.78136, -2.71681, -2.65595,
		-2.59852, -2.54430, -2.49306, -2.44460, -2.39874, -2.35530, -2.31411, -2.27503,
		-2.23791, -2.20261, -2.16903, -2.13703, -2.10651, -2.07738, -2.04953, -2.02288,
		-1.99736, -1.97288, -1.94937, -1.92676, -1.90501, -1.88404, -1.86381, -1.84426,
		-1.82536, -1.80705, -1.78929, -1.77205, -1.75530, -1.73899, -1.72311, -1.70762,
		-1.69249, -1.67770, -1.66323, -1.64906, -1.63516, -1.62151, -1.60811, -1.59493,
		-1.58196, -1.56918, -1.55659, -1.54416, -1.53189, -1.51977, -1.50778, -1.49593,
		-1.48419, -1.47256, -1.46103, -1.44960, -1.43827, -1.42701, -1.41583, -1.40473,
		-1.39369, -1.38272, -1.37181, -1.36095, -1.35014, -1.33938, -1.32866, -1.31799,
		-1.30736, -1.29676, -1.28619, -1.27566, -1.26515, -1.25468, -1.24422
	};

	float u[2 * 200] = {
		// This is the first signal
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000,
		2.0000, 1.9899, 1.9798, 1.9697, 1.9596, 1.9495, 1.9394, 1.9293, 1.9192, 1.9091,
		1.8990, 1.8889, 1.8788, 1.8687, 1.8586, 1.8485, 1.8384, 1.8283, 1.8182, 1.8081,
		1.7980, 1.7879, 1.7778, 1.7677, 1.7576, 1.7475, 1.7374, 1.7273, 1.7172, 1.7071,
		1.6970, 1.6869, 1.6768, 1.6667, 1.6566, 1.6465, 1.6364, 1.6263, 1.6162, 1.6061,
		1.5960, 1.5859, 1.5758, 1.5657, 1.5556, 1.5455, 1.5354, 1.5253, 1.5152, 1.5051,
		1.4949, 1.4848, 1.4747, 1.4646, 1.4545, 1.4444, 1.4343, 1.4242, 1.4141, 1.4040,
		1.3939, 1.3838, 1.3737, 1.3636, 1.3535, 1.3434, 1.3333, 1.3232, 1.3131, 1.3030,
		1.2929, 1.2828, 1.2727, 1.2626, 1.2525, 1.2424, 1.2323, 1.2222, 1.2121, 1.2020,
		1.1919, 1.1818, 1.1717, 1.1616, 1.1515, 1.1414, 1.1313, 1.1212, 1.1111, 1.1010,
		1.0909, 1.0808, 1.0707, 1.0606, 1.0505, 1.0404, 1.0303, 1.0202, 1.0101, 1.0000,

		// This is the second signal
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000,
		10.0000, 4.0000, 3.9798, 3.9596, 3.9394, 3.9192, 3.8990, 3.8788, 3.8586, 3.8384,
		3.8182, 3.7980, 3.7778, 3.7576, 3.7374, 3.7172, 3.6970, 3.6768, 3.6566, 3.6364,
		3.6162, 3.5960, 3.5758, 3.5556, 3.5354, 3.5152, 3.4949, 3.4747, 3.4545, 3.4343,
		3.4141, 3.3939, 3.3737, 3.3535, 3.3333, 3.3131, 3.2929, 3.2727, 3.2525, 3.2323,
		3.2121, 3.1919, 3.1717, 3.1515, 3.1313, 3.1111, 3.0909, 3.0707, 3.0505, 3.0303,
		3.0101, 2.9899, 2.9697, 2.9495, 2.9293, 2.9091, 2.8889, 2.8687, 2.8485, 2.8283,
		2.8081, 2.7879, 2.7677, 2.7475, 2.7273, 2.7071, 2.6869, 2.6667, 2.6465, 2.6263,
		2.6061, 2.5859, 2.5657, 2.5455, 2.5253, 2.5051, 2.4848, 2.4646, 2.4444, 2.4242,
		2.4040, 2.3838, 2.3636, 2.3434, 2.3232, 2.3030, 2.2828, 2.2626, 2.2424, 2.2222,
		2.2020, 2.1818, 2.1616, 2.1414, 2.1212, 2.1010, 2.0808, 2.0606, 2.0404, 2.0202,
		2.0000
	};

	float A[2 * 2];
	float B[2 * 2];
	float C[2 * 2];

	era(u, y, 2, 200, A, B, C, 2, 2); // State space model

	printf("Matrix: B:\n");
	print(B, 2, 2);
	printf("Matrix: C:\n");
	print(C, 2, 2);
	printf("Matrix: A:\n");
	print(A, 2, 2);
}

/*
 * GNU Octave code:
 *   These functions needs Matavecontrol and Mataveid from GitHub
 * 	 sys = ss(0, [0 1; -3 -5], eye(2), eye(2));
	 u = [linspace(5, 5, 100) linspace(2, 1, 100)];
	 t = linspace(0, 20, 200);
	 y = lsim(sys, [u;2*u], t);
	 sysd = okid([u;2*u], y, t(2), 1); % Select 2
 */

void test_okid(void)
{
	// Input
	float u[2 * 10] = { 5, 1, 2, 3, 5, 1, 5, 6, 8, 1, 7, 1, 56, 1, 5, 3, 1, 5, 6, 1 };
	// Output
	float y[2 * 10] = { 3, 51, 3, 2, 6, 7, 8, 9, 1, 4, 1, 3, 1, 4, 5, 1, 7, 8, 1, 10 };

	// Impulse response - Markov parameters
	float s[2 * 10];

	okid(u, y, s, 2, 10);
	printf("Markov parameters:\n");
	print(s, 2, 10);
}

// Dimensions for input and output
#define YDIM 1
#define RDIM 1

// Recursive Least Squares with kalman filter - It's recommended to have NP = NZ = NZE
#define NP 5
#define NZ 5
#define NZE 5
#define ADIM NP
#define forgetting 0.99
#define Pq 1000

void test_rls(void)
{
	/*
	 * Assume that we reading u(k), and y(k) from an ADC. In this case, we have
	 * a vector of u's and y's, but in real time running, we will not have this vector.
	 */
	float input[100] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
	}; // Input signal

	float output[100] = {
		0.00000, 0.00477, 0.01786, 0.03761, 0.06264, 0.09174, 0.12390, 0.15827, 0.19412,
		0.23085, 0.26796, 0.30504, 0.34175, 0.37782, 0.41304, 0.44723, 0.48027, 0.51207,
		0.54255, 0.57168, 0.59943, 0.62581, 0.65081, 0.67447, 0.69680, 0.71785, 0.73765,
		0.75625, 0.77369, 0.79004, 0.80533, 0.81963, 0.83297, 0.84541, 0.85701, 0.86780,
		0.87784, 0.88717, 0.89584, 0.90388, 0.91134, 0.91825, 0.92465, 0.93058, 0.93606,
		0.94113, 0.94582, 0.95015, 0.95415, 0.95784, 0.96124, 0.96915, 0.98513, 1.00755,
		1.03503, 1.06640, 1.10064, 1.13692, 1.17453, 1.21288, 1.25148, 1.28993, 1.32790,
		1.36513, 1.40141, 1.43658, 1.47052, 1.50313, 1.53437, 1.56419, 1.59258, 1.61953,
		1.64507, 1.66921, 1.69199, 1.71345, 1.73362, 1.75257, 1.77033, 1.78697, 1.80252,
		1.81706, 1.83062, 1.84327, 1.85505, 1.86601, 1.87621, 1.88568, 1.89447, 1.90263,
		1.91020, 1.91721, 1.92370, 1.92971, 1.93527, 1.94041, 1.94516, 1.94955, 1.95360,
		1.95734
	};

	// Estimation SISO model - Assume that this this is inside the microprocessor's while(1) loop
	float past_e; // The past e
	float past_y; // The past y
	float past_u; // The past u
	float phi[NP + NZ + NZE];
	float P[(NP + NZ + NZE) * (NP + NZ + NZE)];
	float theta[NP + NZ +
		    NZE]; // Remember that total length is POLY_LENGTH*3 - ALWAYS have these dimensions
	uint8_t count =
		0; // Initial set to 0. Will be counted to 2 then stop. Set count = 0 again and the algorithm starts over
	for (uint8_t i = 0; i < sizeof(output) / sizeof(output[0]); i++) {
		rls(NP, NZ, NZE, theta, input[i], output[i], &count, &past_e, &past_y, &past_u, phi,
		    P, Pq, forgetting);
	}

	// Find our state space model with kalman gain matrix from theta - NP = ADIM
	float A[NP * NP];
	float B[NP * RDIM];
	float C[YDIM * NP];
	float K[NP * YDIM];
	bool integral_action = false;
	theta2ss(A, B, C, theta, K, ADIM, NP, NZ, NZE, integral_action);

	// Print our state space matrix
	printf("System matrix: A\n");
	print(A, NP, NP);

	printf("Input matrix: B\n");
	print(B, NP, RDIM);

	printf("Output matrix: C\n");
	print(C, YDIM, NP);

	printf("Kalman gain matrix: K\n");
	print(K, NP, YDIM);
}

/* Octave code:

	%% Example made by Daniel Mårtensson - 2019-10-08
	%% To run this example. You need to install Matavecontrol and Mataveid

	% Real life model of a water tank
	G = tf([1], [1 2 1]);

	% Time
	t = linspace(0, 10, 100);

	% Input signals
	u1 = linspace(1, 1, 50);
	u2 = linspace(2, 2, 50);
	u = [u1 u2];

	% Simulation of the model - It will give you a graphical plot what type of data you need
	y = lsim(G, u, t);

	% Put some noise to output
	%y = y + 0.03*randn(1, length(y));

	% Identification with recursive least squares and model order 5 and delay 0 and lambda 0.99
	[Gd, Hd, sysd, K] = rls(u, y, 5, 5, 5, t(2)-t(1), 0, 0.99); % sysd contains K at column 2 of B

	% Transfer function to state space
	sysd = tf2ss(Gd)

	disp('Kalman K')
	K


*/

void test_ukf_param_estimation(void)
{
	/*
	 * Hello! This is Square Root Uncented Kalman Filter (SR-UKF) for parameter estimation and this algorithm is successor of Uncented Kalman Filter(UKF)
	 * because UKF had some issues with the cholesky decomposition when it going to find the square root.
	 * SR-UKF come in two papers. The first paper and the second paper. The first paper does not re-compute sigma point matrix for the
	 * observability function H. The second paper re-compute sigma point matrix for the observability function H.
	 * In this algorithm, I assume that the output y and the state has the same dimension and observability function is just an identity matrix.
	 * That's because I want to minimize the input arguments for the SR-UKF function. Less turning parameters, function, constants and so on makes
	 * it easier to use SR-UKF.
	 */

	/* Initial parameters */
	uint8_t L = 3; // How many states we have
	float e = 0.1f; // Tuning factor for noise
	float alpha = 0.1f; // Alpha value - A small number like 0.01 -> 1.0
	float beta = 2.0f; // Beta value - Normally 2 for gaussian noise
	float Re[3 * 3] = {
		e, 0, 0, 0, e, 0, 0, 0, e
	}; // Initial noise covariance matrix - Recommended to use identity matrix
	float Sw[3 * 3] = {
		1, 0, 0, 0, 1, 0, 0, 0, 1
	}; // Initial covariance matrix - Recommended to use identity matrix
	float what[3] = { 0, 0, 0 }; // Estimated parameter vector
	float d[3] = { 0, 0, 0 }; // This is our measurement
	float x[3] = { 4.4f, 6.2f, 1.0f }; // State vector
	float lambda_rls =
		1.0f; // RLS forgetting parameter between 0.0 and 1.0, but very close to 1.0

	/* Our transition function - This is the orifice equation Q = a*sqrt(P2 - P1) for hydraulics */
	void G(float dw[], float x[], float w[])
	{
		dw[0] = w[0] * sqrtf(x[1] - x[0]);

		/* We only need to use w[0] so we assume that w[1] and w[2] will become close to 1.0 */
		dw[1] = w[1] * x[1];
		dw[2] = w[2] * x[2];
	}

	/* Do SR-UKF */
	float WHAT[100 * 3];
	float E[100 * 3];
	float noise[1];
	float dw[3];
	for (uint32_t i = 0; i < 100; i++) {
		/* Create 1 noise */
		randn(noise, 1, 0.0f, 1.0f);

		/* Assume that this is our measurement */
		d[0] = 5.0f + e * noise[0];

		/* This is just to make sure w[1] and w[2] becomes close to 1.0 */
		d[1] = x[1];
		d[2] = x[2];

		/* Estimate new parameter - We assume that our x state vector remains constant */
		sr_ukf_parameter_estimation(d, what, Re, x, G, lambda_rls, Sw, alpha, beta, L);

		/* Save the estimated parameter */
		for (uint8_t j = 0; j < L; j++)
			WHAT[i * 3 + j] = what[j];

		/* Measure the error */
		G(dw, x, what);
		for (uint8_t j = 0; j < L; j++)
			E[i * 3 + j] = fabsf(d[j] - dw[j]);
	}

	printf("Estimated parameters: \n");
	print(WHAT, 100, 3);

	printf("Error: \n");
	print(E, 100, 3);
}
