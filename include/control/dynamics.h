/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */
#include <stdbool.h>
#include <stdint.h>

struct pid {
	float x[3];
	float y[3];
	float Kp, Ki, Kd, d;
};

enum pid_imc_mode { PID_IMC_AGGRESSIVE, PID_IMC_MODERATE, PID_IMC_CONSERVATIVE };

void pid_init(struct pid *self);
void pid_set_gains(struct pid *self, float Kp, float Ki, float Kd, float d);
void pid_set_from_imc(struct pid *self, float aggr, float process_gain, float time_constant,
		      float dead_time, enum pid_imc_mode mode);
float pid_step(struct pid *self, float e);

/**
 * \brief Model predictive control
 * \param A State matrix
 * \param B Control to state matrix
 * \param C State to output matrix
 * \param x State vector
 * \param u Control action
 * \param r Reference
 * \param ADIM Size of A matrix
 * \param YDIM Size of plant output vector
 * \param RDIM Size of input vector
 * \param HORIZON Horizon
 * \param ITERATION_LIMIT Number of iterations
 * \param has_integration set to true is system has an integration behavior
 * \retval 0 Success
 * \retval -EINVAL Invalid arguments
 */
int mpc(float A[], float B[], float C[], float x[], float u[], const float *const r, uint8_t ADIM,
	uint8_t YDIM, uint8_t RDIM, uint8_t HORIZON, uint8_t ITERATION_LIMIT, bool has_integration);
/**
 * \brief Linear kalman filter state update
 * \details
 *   x = Ax + Bu + Ky - KCx
 * \param xout Output state vector after update [ADIM * 1]
 * \param A State transition matrix [ADIM * ADIM]
 * \param x State vector [ADIM * 1]
 * \param B System input to state transition matrix [ADIM * RDIM]
 * \param u Input to the system [RDIM * 1]
 * \param K Kalman gain matrix precomputed by kalman algorithm [ADIM * YDIM]
 * \param y Measurement vector (noisy) [YDIM * 1]
 * \param C State to output transition matrix [YDIM * ADIM]
 * \param ADIM State matrix dimensions
 * \param YDIM Output vector dimension
 * \param RDIM Input vector dimension
 **/
void kalman(float *xout, const float *const A, const float *x, const float *const B,
	    const float *const u, const float *const K, const float *const y, const float *const C,
	    uint8_t ADIM, uint8_t YDIM, uint8_t RDIM);
/**
 * \brief Linear Quadratic Integral control algorithm
 * \details
 *   This regulator works on input signal that is of the negative error feedback
 *   form -(r - y) but uses the same system dynamics as the system we are
 *   controlling. This means that what we are trying to do is always bring the
 *   output (error) to zero. Reference point is used to compute the error passed
 *   to the regulator.
 *
 *   xi += e
 *
 *   u = -(Lx + Li * (xi + err))
 * \param u Input signal vector [RDIM * 1]
 * \param L LQR Control Law [RDIM * ADIM]
 * \param x Error state vector [ADIM]
 * \param Li LQI Integral law [RDIM * ADIM]
 * \param xi Integral vector [YDIM]
 * \param err Error vector [YDIM * 1]
 * \param ADIM Dimensions of A matrix
 * \param YDIM Dimensions of y
 * \param RDIM Dimensions of u
 * \param ANTI_WINDUP Anti-windup enabled if set to true (code needs review. Don't use)
 **/
void lqi(float *u, const float *const L, const float *const x, const float *const Li, float *xi,
	 const float *const err, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t ANTI_WINDUP);
/**
 * \brief Adaptive Model Reference Control
 * \details
 * We assume that the reference model is Gm(s) = 1.
 *
 * u = r*integral(-gain*r*e) - y*integral(gain*y*e);
 * Where: u = input to our system
 *      : r = reference
 *      : y = output
 *      : gain = learning factor e.g 0.00001
 *      : e = Model error y-r
 *
 * This can be used with multiple outputs and references
 * \param limit Integral limit
 * \param gain Controller gain
 * \param y Measurement vector
 * \param u Control action
 * \param r Reference
 * \param I1 Integral1
 * \param I2 Integral2
 * \param RDIM size of reference and output vector
 **/
void mrac(float limit, float gain, float *y, float *u, float *r, float *I1, float *I2,
	  uint8_t RDIM);

/**
 * \brief SISO transfer function to state space canonical form
 * \details
 * Convert SISO transfer function parameters theta vector into a state space
 * model on Observable Canonical Form.
 *
 * Vector theta can be found from rls function.
 *
 * It's recommended that NP = NZ = NZE = ADIM
 * No integral action:
 * A [ADIM*ADIM]
 * B [ADIM*1]
 * C [1*ADIM]
 * K [ADIM*1]
 * theta [NP+NZ+NZE]
 *
 * Width integral action:
 * A [(ADIM+1)*(ADIM+1)]
 * B [(ADIM+1)*1]
 * C [1*(ADIM+1)]
 * K [(ADIM+1)*1]
 *
 * [A, B, C, D] = tf2ss(theta)
 * \param A Output system matrix A
 * \param B Output system matrix B
 * \param C Output system matrix C
 * \param K Output Kalman gain K
 * \param theta Poles and zeros of the transfer function
 * \param ADIM size of A matrix
 * \param NP number of poles
 * \param NZ number of zeros
 * \param NZE
 * \param integral_action Set to true if system has integral action
 **/
void theta2ss(float *A, float *B, float *C, float *K, const float *const theta, uint8_t ADIM,
	      uint8_t NP, uint8_t NZ, uint8_t NZE, bool integral_action);

/**
 * \brief Check the stability of the matrix A by checking the eigenvalues
 * \retval true if A is stable.
 * \retval false is A is unstable
 */
bool is_stable(const float *const A, uint8_t ADIM);
/**
 * \brief Continuous to discrete transformation
 * \details
 *   Turn A and B into discrete form with given sample time
 * \param Ad discrete output system A matrix [ADIM * ADIM]
 * \param Bd discrete output system B matrix [ADIM * RDIM]
 * \param A System A matrix [ADIM * ADIM]
 * \param B System B matrix [ADIM * RDIM]
 * \param ADIM Size of system A matrix (rows and columns)
 * \param RDIM Number of rows in B matrix
 * \param sampleTime Sampling time
 **/
void c2d(float *Ad, float *Bd, const float *const A, const float *const B, uint8_t ADIM,
	 uint8_t RDIM, float sampleTime);
