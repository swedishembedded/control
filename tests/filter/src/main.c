/*
 ============================================================================
 Name        : filtfilt.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Filter the y array with filtfilt
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unity.h>
#include <control/filter.h>
#include <control/misc.h>

#define DEG2RAD(x) (x * (M_PI / 180))

void test_filtfilt(void)
{
	float y[200] = {
		0.2986021,  -0.0577614, -0.0383242, 0.1164948,	0.1482280,  0.2002007,	0.2132362,
		0.2383201,  0.3558672,	0.4308540,  0.5137154,	0.5687722,  0.5807755,	0.6015722,
		0.5195360,  0.9031069,	0.7208930,  0.8763094,	0.8096338,  0.5337779,	0.9762575,
		0.9306229,  0.7027062,	0.7298749,  0.8324121,	1.0054024,  0.8734718,	0.9677193,
		1.0558149,  0.9826274,	0.9194703,  0.9999371,	1.0433159,  1.0801228,	0.9766006,
		0.9231713,  1.0893196,	0.9722423,  1.1289529,	0.9418383,  0.8374435,	0.8162144,
		0.9320393,  0.6498831,	0.7701721,  0.8080873,	0.7674149,  0.6437390,	0.8658278,
		0.4597700,  0.5528558,	0.6905087,  0.5721611,	0.5189363,  0.4474690,	0.1324550,
		0.2007300,  0.2707765,	0.2520129,  0.1625879,	-0.0088429, 0.1058720,	0.0434291,
		-0.1593475, -0.0904459, -0.1339623, -0.0985645, -0.1545048, -0.3290014, -0.2351123,
		-0.3198143, -0.3728968, -0.4787458, -0.6042771, -0.4541768, -0.5406007, -0.6440048,
		-0.8101873, -0.8237520, -0.8086112, -0.7836505, -0.8909165, -0.8157668, -0.8585342,
		-0.8600261, -1.0248668, -1.0274373, -0.8873499, -0.8808206, -1.0070624, -1.0305323,
		-0.8660065, -1.1450002, -0.9617550, -0.9493855, -0.9656046, -1.0302306, -0.9477365,
		-0.7676748, -1.0060407, -1.0036219, -0.8562681, -0.8660147, -0.9030836, -0.6688155,
		-0.8541154, -0.7712126, -0.8206464, -0.6643524, -0.9318013, -0.8242707, -0.4562556,
		-0.5070651, -0.5163247, -0.4505979, -0.4420079, -0.4679928, -0.4806282, -0.3775785,
		-0.3629770, -0.1796592, -0.3402945, -0.0251914, -0.0540721, 0.0638847,	-0.0520884,
		-0.0269831, -0.0145537, 0.1443569,  0.2834443,	0.1325746,  0.1718424,	0.4806738,
		0.4524129,  0.3351090,	0.4667744,  0.5135405,	0.7313122,  0.7096539,	0.7326623,
		0.6152441,  0.7243686,	0.7527686,  0.6851885,	0.8091517,  0.8274868,	0.8928504,
		1.0040233,  0.7126505,	0.9985160,  0.7746326,	0.9790915,  0.8951794,	0.9036120,
		0.9650351,  1.1285304,	0.9635931,  1.1391099,	0.8256712,  0.9703603,	1.0569489,
		1.1486799,  0.8958082,	1.0795558,  1.0556795,	0.8994170,  0.8077465,	1.0636900,
		0.8205777,  0.8422575,	0.6829634,  0.7024461,	0.6307279,  0.7007650,	0.7144214,
		0.5199631,  0.6163695,	0.4861224,  0.5100844,	0.4305812,  0.3572576,	0.1921604,
		0.2766891,  0.2009808,	0.2174102,  0.1082726,	0.2130828,  0.0371421,	-0.0692708,
		-0.0336184, -0.2543395, -0.2175047, -0.3404012, -0.2840302, -0.5171487, -0.4282719,
		-0.3427022, -0.3475412, -0.7124227, -0.7152642
	};

	float t[2] = { 0, 0.050251 }; // We only need 2 samples of t!

	filtfilt(y, t, 200, 0.08); // 200 = length of y array, 0.08 = tuning parameter
	print(y, 200, 1);
}

void test_mcs_filter(void)
{
	// Assume that we have a normal distributed vector with dimension row_x = 3
	uint8_t row_x = 3;
	float X0[2000];
	float X1[2000];
	float X2[2000];
	float x[3];
	uint16_t length = 2000;
	randn(X0, length, 20, 1); // Mean 20, Std 1
	randn(X1, length, -4, 1); // Mean -4, Std 1
	randn(X2, length, 6, 1); // Mean 6, Sted 1

	// Then we create a matrix P with row_x*2*column_p
	float P[6 * 500];
	uint16_t column_p =
		500; // You want to keep this small as possible, but still fill every index
	float index_factor =
		20; // Large number gives better precision but it requires larger column_p

	// Important to clean the P matrix first before filling P matrix with x vector.
	mcs_clean(P, column_p, row_x);
	for (uint16_t i = 0; i < length; i++) {
		x[0] = X0[i];
		x[1] = X1[i];
		x[2] = X2[i];
		mcs_collect(P, column_p, x, row_x, index_factor);
	}

	// Estimate which vector x is most applied in P
	mcs_estimate(P, column_p, x, row_x);

	// Print P and try to change index_factor and column_p for best fit.
	print(P, row_x * 2, column_p);

	// Don't forget to clean the filter before you are using it again
	mcs_clean(P, column_p, row_x);

	// Print best estimation
	printf("Final result: x[0] = %f, x[1] = %f, x[2] = %f\n", x[0], x[1], x[2]);
}

void test_ukf_filter(void)
{
	/*
	 * Hello! This is Square Root Uncented Kalman Filter (SR-UKF) for state estimation and this algorithm is successor of Uncented Kalman Filter(UKF)
	 * because UKF had some issues with the cholesky decomposition when it going to find the square root.
	 * SR-UKF come in two papers. The first paper and the second paper. The first paper does not re-compute sigma point matrix for the
	 * observability function H. The second paper re-compute sigma point matrix for the observability function H.
	 * In this algorithm, I assume that the output y and the state has the same dimension and observability function is just an identity matrix.
	 * That's because I want to minimize the input arguments for the SR-UKF function. Less turning parameters, function, constants and so on makes
	 * it easier to use SR-UKF.
	 */

	/* Initial parameters */
	// How many states we have
	uint8_t L = 3;
	// Tuning factor for noise
	float r = 0.1f;
	// Tuning factor for disturbance
	float q = 0.01f;
	// Alpha value - A small number like 0.01 -> 1.0
	float alpha = 0.1f;
	// Beta value - Normally 2 for gaussian noise
	float beta = 2.0f;
	// Initial disturbance covariance matrix - Recommended to use identity matrix
	float Rv[3 * 3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	// Initial noise covariance matrix - Recommended to use identity matrix
	float Rn[3 * 3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	// Initial covariance matrix - Recommended to use identity matrix
	float S[3 * 3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	float xhat[3] = { 0, 0, 1 }; // Estimated state vector
	float y[3] = { 0, 0, 0 }; // This is our measurement
	float u[3] = {
		0, 0, 0
	}; // u is not used in this example due to the transition function not using an input signal
	float x[3] = { 0, 0, 0 }; // State vector for the system (unknown in reality)

	/* Our transition function */
	void F(float dx[], float x[], float u[])
	{
		dx[0] = x[1];
		dx[1] = x[2];
		dx[2] = 0.05 * x[0] * (x[1] + x[2]);
	}

	/* Save arrays */
	float X[200 * 3];
	float XHAT[200 * 3];
	float Y[200 * 3];

	/* Measurement noise */
	float noise[3];

	/* Disturbances */
	float phase[3] = { 90, 180, 140 };
	float amplitude[3] = { 0.1f, 0.1f, 0.1f };

	/* Derivative */
	float dx[3];

	/* SR-UKF simulation */
	for (uint32_t i = 0; i < 200; i++) {
		/* Create noise for the measurement */
		randn(noise, L, 0.0f, 1.0f);
		for (uint8_t j = 0; j < L; j++)
			y[j] = x[j] + r * noise[j];

		/* Save measurement */
		for (uint8_t j = 0; j < L; j++)
			Y[i * 3 + j] = y[j];

		/* Save actual state */
		for (uint8_t j = 0; j < L; j++)
			X[i * 3 + j] = x[j];

		/* Estimate new state */
		sr_ukf_state_estimation(y, xhat, Rn, Rv, u, F, S, alpha, beta, L);

		/* Save the estimated state */
		for (uint8_t j = 0; j < L; j++) {
			XHAT[i * 3 + j] = xhat[j];
		}

		/* Update the state with disturbance */
		F(dx, x, u);
		for (uint8_t j = 0; j < L; j++) {
			x[j] = dx[j] + q * amplitude[j] * sinf(i + phase[j]);
		}
	}

	/* Data */
	printf("Actual state:\n");
	print(X, 200, 3);

	printf("Estimated state:\n");
	print(XHAT, 200, 3);

	printf("Measurement:\n");
	print(Y, 200, 3);
}
