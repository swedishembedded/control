/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/dynamics.h"
#include "control/model/dc_motor.h"
};

void step_for(struct model_dc_motor *m, unsigned int steps)
{
	for (unsigned int i = 0; i < steps; i++) {
		model_dc_motor_step(m);
	}
}

TEST(DCMotorTest, MotorSpinup)
{
	struct model_dc_motor m;
	model_dc_motor_init(&m);
	m.b = 0.0;
	m.u[0] = 0;
	m.K = 0.123;
	step_for(&m, 20);
	m.u[0] = 1;
	step_for(&m, 2000);
	// with no friction, motor angular velocity will converge to u/K
	ASSERT_NEAR(m.u[0], model_dc_motor_get_omega(&m) * m.K, 0.01);
}
