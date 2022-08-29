/* SPDX-License-Identifier: Apache-2.0 */
/**
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 **/

#pragma once

/**
 * \brief DC Motor model
 * \details
 *   x[k+1] = Ax + Bu
 *   y[k+1] = Cx + Du
 **/
struct model_dc_motor {
	/** Discrete state transition matrix */
	float A[4];
	/** Discrete measurement transition matrix */
	float B[2];
	/** State to measurement transition */
	float C[2];
	/** Input to measurement (feedforward) transition */
	float D[1];
	/** Moment of inertia of the rotor     kg.m^2 */
	float J;
	/** Motor viscous friction constant    N.m.s */
	float b;
	/** Electromotive force constant       V/rad/sec */
	/** Mmotor torque constant             N.m/Amp */
	float K;
	/** Electric resistance                Ohm */
	float R;
	/** Electric inductance                H */
	float L;
	/** Sampling time for the discrete system */
	float Ts;
	/** State:
	 *  x[0] = angular velocity (Rad/sec)
	 *  x[1] = winding current (A)
	 **/
	float x[2];
	/** Measurement: y[0] = Angular velocity (Rad/sec) */
	float y[1];
	/** Input: u[0] = Motor voltage (V) */
	float u[1];
	/** Rotor angle (Rad) */
	float position;
	/** Motor limits */
	struct {
		float voltage;
		float current;
	} limits;
};

/**
 * \brief Initialize motor model with default settings
 * \param self Motor model instance
 **/
void model_dc_motor_init(struct model_dc_motor *self);
/**
 * \brief Step simulation one time step
 * \param self Motor model
 **/
void model_dc_motor_step(struct model_dc_motor *self);
/**
 * \brief Get motor angular velocity in Rad/sec
 * \param self Motor model
 **/
float model_dc_motor_get_omega(struct model_dc_motor *self);
/**
 * \brief Set motor voltage in volts
 * \param self Motor model
 * \param voltage Winding voltage (V)
 **/
void model_dc_motor_set_voltage(struct model_dc_motor *self, float voltage);
