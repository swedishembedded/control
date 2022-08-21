// SPDX-License-Identifier: MIT
/**
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 **/
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "control/motion.h"

void motion_profile_init(struct motion_profile *self, float max_acc, float max_speed,
                         float max_dec) {
	memset(self, 0, sizeof(*self));
	self->acc = fabsf(max_acc);
	self->vmax = fabsf(max_speed);
	self->dec = fabsf(max_dec);
	self->sign = 1;
}

void motion_profile_plan_move(struct motion_profile *self, float dstart,
                              float vstart, float dend, float vend) {
	// solution generated by solving following two systems of equations
	/*
	  syms acc dec t1 t2 t3 vmax dist vstart vend
	  f1 = (vstart * t1 + (acc * t1^2)/2) + (vend * t3 + (dec * t3^2) / 2) - dist
	  f2 = vstart + acc * t1 - dec * t3 - vend
	  s = solve(f1 == 0, f2 == 0, [t1, t3])

	  syms acc dec t1 t2 t3 vmax dist vstart vend
	  f1 = (vstart * t1 + (acc * t1^2)/2) + vmax * t2 + (vend * t3 + (dec * t3^2) / 2)
	  - dist f2 = vstart + acc * t1 - dec * t3 - vend f3 = vstart + acc * t1 - vmax
	  solve(f1 == 0, f2 == 0, f3 == 0, [t1, t2, t3])
	*/
	if(dend >= dstart) {
		self->sign = 1;
	} else {
		self->sign = -1;
	}

	float acc = self->sign * self->acc;
	float dec = self->sign * self->dec;
	float vmax = self->sign * self->vmax;
	float dist = dend - dstart;

	self->t1 = (-vend * (acc + dec) +
	            sqrtf((acc + dec) * (2 * acc * dec * dist + acc * vend * vend +
	                                 dec * vstart * vstart)) +
	            (acc + dec) * (vend - vstart)) /
	           (acc * (acc + dec));
	self->t2 = 0;
	self->t3 = (-vend * (acc + dec) +
	            sqrtf((acc + dec) * (2 * acc * dec * dist + acc * vend * vend +
	                                 dec * vstart * vstart))) /
	           (dec * (acc + dec));

	self->v2 = vstart + acc * self->t1;
	if(fabsf(self->v2) > fabsf(vmax)) {
		self->t1 = (vmax - vstart) / acc;
		self->t2 = (2 * acc * dec * dist + acc * vend * vend - acc * vmax * vmax -
		            dec * vmax * vmax + dec * vstart * vstart) /
		           (2 * acc * dec * vmax);
		self->t3 = -(vend - vmax) / dec;
	}
	self->total_time = self->t1 + self->t2 + self->t3;

	self->v1 = vstart;
	self->v2 = vstart + acc * self->t1;
	self->v3 = vend;

	self->d1 = self->v1 * self->t1 + (acc * self->t1 * self->t1) / 2.f;
	self->d2 = self->v2 * self->t2;
	self->d3 = self->v3 * self->t3 + (dec * self->t3 * self->t3) / 2.f;
	self->total_distance = self->d1 + self->d2 + self->d3;

	self->start_position = dstart;
	self->move_planned = true;
}

motion_profile_state_t motion_profile_get_pva(struct motion_profile *self, float time, float *pos_sp,
                           float *vel_sp, float *acc_sp) {
	if(!self->move_planned)
		return -1;

	float t = time;
	float v = self->v3;
	float d = self->total_distance + self->v3 * (t - self->total_time);
	float a = 0;

	float acc = self->sign * self->acc;
	float dec = self->sign * self->dec;

	motion_profile_state_t state = MOTION_PROFILE_UNINITIALIZED;

	if(t < self->t1) {
		v = self->v1 + acc * t;
		d = self->v1 * t + (acc * t * t) / 2;
		a = acc;
		state = MOTION_PROFILE_ACCELERATING;
	} else if(t >= self->t1 && t < (self->t1 + self->t2)) {
		v = self->v2;
		d = self->d1 + (t - self->t1) * self->v2;
		a = 0;
		state = MOTION_PROFILE_CRUISING;
	} else if(t >= (self->t1 + self->t2) && t < self->total_time) {
		v = self->v2 - dec * (t - self->t1 - self->t2);
		d = self->total_distance - self->v3 * (self->total_time - t) -
		    (dec * (self->total_time - t) * (self->total_time - t)) / 2;
		a = -dec;
		state = MOTION_PROFILE_DECELERATING;
	} else {
		state = MOTION_PROFILE_COMPLETED;
	}

	// just a precaution. Not that it will ever happen during normal operation..
	if(isnan(v) || isnan(d))
		return MOTION_PROFILE_INVALID;

	*vel_sp = v;
	*pos_sp = self->start_position + d;
	*acc_sp = a;

	return state;
}

float motion_profile_get_traversal_time(struct motion_profile *self) {
	return self->total_time;
}
