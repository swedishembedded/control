// SPDX-License-Identifier: Apache-2.0
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <string.h>
#include <math.h>

#include "control/dynamics.h"

void pid_init(struct pid *self)
{
	memset(self, 0, sizeof(*self));
}

void pid_set_from_imc(struct pid *self, float aggr, float process_gain, float time_constant,
		      float dead_time)
{
	float tc = fmaxf((0.1 * aggr) * time_constant, (0.8 * aggr) * dead_time);
	float kc =
		(1.0 / process_gain) * ((time_constant + 0.5 * dead_time) / (tc + 0.5 * dead_time));
	float ti = time_constant + 0.5 * dead_time;
	float td = (time_constant * dead_time) / (2.0 * time_constant + dead_time);

	self->Kp = kc;
	self->Ki = kc / ti;
	self->Kd = kc * td;
}

void pid_set_gains(struct pid *self, float Kp, float Ki, float Kd, float d)
{
	self->Kp = Kp;
	self->Ki = Ki;
	self->Kd = Kd;
	self->d = d;
}

float pid_step(struct pid *self, float e)
{
	float Kp = self->Kp;
	float Ki = self->Ki;
	float Kd = self->Kd;
	float d = self->d;
	float gain = -Kd * d + Kd + Kp;

	self->x[2] = self->x[1];
	self->x[1] = self->x[0];
	self->x[0] = e;
	self->y[2] = self->y[1];
	self->y[1] = self->y[0];
	self->y[0] = 0;
	self->y[0] += self->x[0] * (1) * gain;
	self->y[0] += self->x[1] *
		      ((2 * Kd * d - 2 * Kd + Ki - Kp * d - Kp) / (-Kd * d + Kd + Kp)) * gain;
	self->y[0] += self->x[2] * ((-Kd * d + Kd - Ki * d + Kp * d) / (-Kd * d + Kd + Kp)) * gain;
	self->y[0] -= self->y[1] * (-d - 1);
	self->y[0] -= self->y[2] * (d);
	return self->y[0];
}
