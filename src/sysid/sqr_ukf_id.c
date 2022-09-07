// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */
#include "control/linalg.h"
#include "control/sysid.h"

#include <errno.h>
#include <math.h>
#include <string.h>

static void scale_Sw_with_lambda_rls_factor(float Sw[], float lambda_rls, uint8_t L)
{
	/* Create the size M */
	uint8_t M = 2 * L;

	/* Apply scalar factor to Sw */
	for (uint8_t i = 0; i < M; i++)
		Sw[i] *= 1.0f / sqrtf(lambda_rls);
}

static void create_sigma_point_matrix(float W[], float what[], float Sw[], float alpha, float kappa,
				      uint8_t L)
{
	/* Create the size N and K */
	uint8_t N = 2 * L + 1;
	uint8_t K = L + 1;

	/* Compute lambda and gamma parameters */
	float lambda = alpha * alpha * ((float)L + kappa) - (float)L;
	float gamma = sqrtf((float)L + lambda);

	/* Insert first column in W */
	for (uint8_t i = 0; i < L; i++) {
		W[i * N] = what[i];
	}

	/* Insert in to the middle of the columns - Positive */
	for (uint8_t j = 1; j < K; j++) {
		for (uint8_t i = 0; i < L; i++) {
			W[i * N + j] = what[i] + gamma * Sw[i * L + j - 1];
		}
	}

	/* Insert in the rest of the columns - Negative */
	for (uint8_t j = K; j < N; j++) {
		for (uint8_t i = 0; i < L; i++) {
			W[i * N + j] = what[i] - gamma * Sw[i * L + j - K];
		}
	}
}

static void compute_transistion_function(float D[], float W[], float x[],
					 void (*G)(float[], float[], float[]), uint8_t L)
{
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Create the derivative state and state vector */
	float dw[L];
	float w[L];

	/* Call the F transition function with W matrix */
	for (uint8_t j = 0; j < N; j++) {
		/* Fill the state vector w with every row from W */
		for (uint8_t i = 0; i < L; i++)
			w[i] = W[i * N + j];

		/* Call the transition function */
		G(dw, x, w);

		/* Get dw into D */
		for (uint8_t i = 0; i < L; i++)
			D[i * N + j] = dw[i];
	}
}

static void multiply_sigma_point_matrix_to_weights(float dhat[], float D[], float Wm[], uint8_t L)
{
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Clear dhat */
	memset(dhat, 0, L * sizeof(float));

	/* Multiply dhat = Wm*D */
	for (uint8_t j = 0; j < N; j++)
		for (uint8_t i = 0; i < L; i++)
			dhat[i] += Wm[j] * D[i * N + j];
}

static void create_state_estimation_error_covariance_matrix(float Sd[], float Wc[], float D[],
							    float dhat[], float Re[], uint8_t L)
{
	/* Create the size N, M and K */
	uint8_t N = 2 * L + 1;
	uint8_t M = 2 * L + L;
	uint8_t K = 2 * L;

	/* Square root parameter of index 1 */
	float weight1 = sqrtf(fabsf(Wc[1]));

	/* Create [Q, R_] = qr(A') */
	float AT[L * M];
	float Q[M * M];
	float R[M * L];

	for (uint8_t j = 0; j < K; j++) {
		for (uint8_t i = 0; i < L; i++) {
			AT[i * M + j] = weight1 * (D[i * N + j + 1] - dhat[i]);
		}
	}
	for (uint8_t j = K; j < M; j++) {
		for (uint8_t i = 0; i < L; i++) {
			AT[i * M + j] = sqrtf(Re[i * L + j - K]);
		}
	}

	/* We need to do transpose on A according to the SR-UKF paper */
	tran(AT, AT, L, M);

	/* Solve [Q, R] = qr(A') but we only need R matrix */
	qr(AT, Q, R, M, L, true);

	/* Get the upper triangular of R according to the SR-UKF paper */
	memcpy(Sd, R, L * L * sizeof(float));

	/* Perform cholesky update on Sd */
	float b[L];

	for (uint8_t i = 0; i < L; i++)
		b[i] = D[i * N] - dhat[i];

	bool rank_one_update = Wc[0] < 0.0f ? false : true;

	cholupdate(Sd, b, L, rank_one_update);
}

static void create_state_cross_covariance_matrix(float Pwd[], float Wc[], float W[], float D[],
						 float what[], float dhat[], uint8_t L)
{
	/* Create the size N and K */
	uint8_t N = 2 * L + 1;
	uint8_t K = 2 * L;

	/* clear P */
	memset(Pwd, 0, K * sizeof(float));

	/* Subtract the matrices */
	for (uint8_t j = 0; j < N; j++) {
		for (uint8_t i = 0; i < L; i++) {
			W[i * N + j] -= what[i];
			D[i * N + j] -= dhat[i];
		}
	}

	/* Create diagonal matrix */
	float diagonal_W[N * N];

	memset(diagonal_W, 0, N * N * sizeof(float));
	for (uint8_t i = 0; i < N; i++)
		diagonal_W[i * N + i] = Wc[i];

	/* Do Pwd = W*diagonal_W*D' */
	tran(D, D, L, N);
	float diagonal_WD[N * L];

	mul(diagonal_WD, diagonal_W, D, N, N, N, L);
	mul(Pwd, W, diagonal_WD, L, N, N, L);
}

// Sw, what, dhat, d, Sd, Pwd, L
static void update_state_covarariance_matrix_and_state_estimation_vector(float Sw[], float what[],
									 float dhat[], float d[],
									 float Sd[], float Pwd[],
									 uint8_t L)
{
	/* Transpose of Sd */
	float SdT[L * L];

	memcpy(SdT, Sd, L * L * sizeof(float));
	tran(SdT, SdT, L, L);

	/* Multiply Sd and Sd' to Sd'Sd */
	float SdTSd[L * L];

	mul(SdTSd, SdT, Sd, L, L, L, L);

	/* Take inverse of Sd'Sd - Inverse is using LUP-decomposition */
	inv(SdTSd, SdTSd, L);

	/* Compute kalman gain K from Sd'Sd * K = Pwd => K = Pwd * inv(SdTSd) */
	float K[L * L];

	mul(K, Pwd, SdTSd, L, L, L, L);

	/* Compute what = what + K*(d - dhat) */
	float ddhat[L];
	float Kd[L];

	for (uint8_t i = 0; i < L; i++)
		ddhat[i] = d[i] - dhat[i];
	mul(Kd, K, ddhat, L, L, L, 1);
	for (uint8_t i = 0; i < L; i++)
		what[i] = what[i] + Kd[i];

	/* Compute U = K*Sd */
	float U[L * L];

	mul(U, K, Sd, L, L, L, L);

	/* Compute Sw = cholupdate(Sw, Uk, -1) because Uk is a vector and U is a matrix */
	float Uk[L];

	for (uint8_t j = 0; j < L; j++) {
		for (uint8_t i = 0; i < L; i++)
			Uk[i] = U[i * L + j];
		cholupdate(Sw, Uk, L, false);
	}
}

static void create_weights(float Wc[], float Wm[], float alpha, float beta, float kappa, uint8_t L)
{
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Compute lambda and gamma parameters */
	float lambda = alpha * alpha * ((float)L + kappa) - (float)L;

	/* Insert at first index */
	Wm[0] = lambda / ((float)L + lambda);
	Wc[0] = Wm[0] + 1 - alpha * alpha + beta;

	/* The rest of the indexes are the same */
	for (uint8_t i = 1; i < N; i++) {
		Wc[i] = 0.5f / ((float)L + lambda);
		Wm[i] = Wc[i];
	}
}

int sqr_ukf_id(float d[], float what[], float Re[], float x[], void (*G)(float[], float[], float[]),
	       float lambda_rls, float Sw[], float alpha, float beta, uint8_t L)
{
	if (L == 0) {
		return -EINVAL;
	}
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Predict: Create the weights */
	float Wc[N];
	float Wm[N];
	float kappa = 3.0f - (float)L; /* kappa is 3 - L for parameter estimation */

	memset(Wc, 0, sizeof(Wc));
	memset(Wm, 0, sizeof(Wm));

	create_weights(Wc, Wm, alpha, beta, kappa, L);

	/* Predict: Scale Sw with lambda_rls */
	scale_Sw_with_lambda_rls_factor(Sw, lambda_rls, L);

	/* Predict: Create sigma point matrix for G function  */
	float W[L * N];

	create_sigma_point_matrix(W, what, Sw, alpha, kappa, L);

	/* Predict: Compute the model G */
	float D[L * N];

	memset(D, 0, sizeof(D));

	compute_transistion_function(D, W, x, G, L);

	/* Predict: Multiply sigma points to weights for dhat */
	float dhat[L];

	multiply_sigma_point_matrix_to_weights(dhat, D, Wm, L);

	/* Update: Create measurement covariance matrix */
	float Sd[L * L];

	create_state_estimation_error_covariance_matrix(Sd, Wc, D, dhat, Re, L);

	/* Update: Create parameter covariance matrix */
	float Pwd[L * L];

	create_state_cross_covariance_matrix(Pwd, Wc, W, D, what, dhat, L);

	/* Update: Perform parameter update and covariance update */
	update_state_covarariance_matrix_and_state_estimation_vector(Sw, what, dhat, d, Sd, Pwd, L);

	return 0;
}
