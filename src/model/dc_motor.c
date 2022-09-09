// SPDX-License-Identifier: Apache-2.0
/**
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 **/

#undef __STRICT_ANSI__

#include "control/linalg.h"
#include "control/model/dc_motor.h"

#include <math.h>
#include <string.h>

void model_dc_motor_init(struct model_dc_motor *self)
{
	memset(self, 0, sizeof(*self));
	self->u[0] = 0.0f;
	self->J = 0.01f;
	self->b = 0.1f;
	self->K = 0.01f;
	self->R = 1.0f;
	self->L = 0.5f;
	self->Ts = 0.1f;
	self->limits.voltage = 24.0f;
	self->limits.current = 10.0f;
}

void model_dc_motor_step(struct model_dc_motor *self)
{
	const float J = self->J;
	const float b = self->b;
	const float K = self->K;
	const float R = self->R;
	const float L = self->L;
	const float Ts = self->Ts;

	// apply limits
	if (self->u[0] < -self->limits.voltage) {
		self->u[0] = -self->limits.voltage;
	}
	if (self->u[0] > self->limits.voltage) {
		self->u[0] = self->limits.voltage;
	}

	self->A[0 * 2 + 0] =
		J * (L + R * Ts) / (powf(K, 2) * powf(Ts, 2) + (J + Ts * b) * (L + R * Ts));
	self->A[0 * 2 + 1] = K * L * Ts / (powf(K, 2) * powf(Ts, 2) + (J + Ts * b) * (L + R * Ts));
	self->A[1 * 2 + 0] = -J * K * Ts / (powf(K, 2) * powf(Ts, 2) + (J + Ts * b) * (L + R * Ts));
	self->A[1 * 2 + 1] =
		L * (J + Ts * b) / (powf(K, 2) * powf(Ts, 2) + (J + Ts * b) * (L + R * Ts));
	self->B[0 * 1 + 0] =
		K * powf(Ts, 2) /
		(J * L + J * R * Ts + powf(K, 2) * powf(Ts, 2) + L * Ts * b + R * powf(Ts, 2) * b);
	self->B[1 * 1 + 0] =
		Ts * (J + Ts * b) /
		(J * L + J * R * Ts + powf(K, 2) * powf(Ts, 2) + L * Ts * b + R * powf(Ts, 2) * b);
	self->C[0 * 2 + 0] = 1;
	self->C[0 * 2 + 1] = 0;
	self->D[0 * 1 + 0] = 0;

	float Ax[2];
	float Bu[2];
	float Cx[1];
	float Du[1];

	mul(Ax, self->A, self->x, 2, 2, 2, 1);
	mul(Bu, self->B, self->u, 2, 1, 1, 1);
	mul(Cx, self->C, self->x, 1, 2, 2, 1);
	mul(Du, self->D, self->u, 1, 1, 1, 1);
	self->x[0] = Ax[0] + Bu[0];
	self->x[1] = Ax[1] + Bu[1];
	self->y[0] = Cx[0] + Du[0];
	// integrate rotor angle
	self->position += self->x[0] * Ts;
	self->position = fmodf(self->position + (float)M_PI, 2.0f * (float)M_PI);
	if (self->position < 0) {
		self->position += 2.0f * (float)M_PI;
	}
	self->position -= (float)M_PI;
}

float model_dc_motor_get_omega(struct model_dc_motor *self)
{
	// this is a simplification because C matrix is unit
	return self->y[0];
}

void model_dc_motor_set_voltage(struct model_dc_motor *self, float voltage)
{
	self->u[0] = voltage;
}
