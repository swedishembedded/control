/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/motion.h"
};

TEST(MotionProfile, ShouldInitializeAbsoluteQuantities)
{
	struct motion_profile p;
	motion_profile_init(&p, -1, -2, -3);
	ASSERT_FLOAT_EQ(1, p.acc);
	ASSERT_FLOAT_EQ(2, p.vmax);
	ASSERT_FLOAT_EQ(3, p.dec);
}

TEST(MotionProfile, ShouldPlanForwardMoveCorrectly)
{
	struct motion_profile p;
	float pos, vel, acc;

	// a simple profile with acceleration of 1, maximum speed of 2 and deceleration of 2
	motion_profile_init(&p, 1, 2, 2);
	// start at distance 3, velocity 0
	// end at distance 12, velocity 0
	motion_profile_plan_move(&p, 3, 0, 12, 0);
	// check position, velocity, acceleration at t == 1
	motion_profile_get_pva(&p, 1, &pos, &vel, &acc);
	ASSERT_FLOAT_EQ(3.5, pos);
	ASSERT_FLOAT_EQ(1, vel);
	ASSERT_FLOAT_EQ(1, acc);

	motion_profile_get_pva(&p, 2, &pos, &vel, &acc);
	ASSERT_FLOAT_EQ(5, pos);
	ASSERT_FLOAT_EQ(2, vel);
	ASSERT_FLOAT_EQ(0, acc);

	ASSERT_FLOAT_EQ(6, motion_profile_get_traversal_time(&p));

	// check that we are stopped at the end
	motion_profile_get_pva(&p, motion_profile_get_traversal_time(&p), &pos, &vel, &acc);
	ASSERT_FLOAT_EQ(12, pos);
	ASSERT_FLOAT_EQ(0, vel);
	ASSERT_FLOAT_EQ(0, acc);
}

TEST(MotionProfile, ShouldPlanReverseMoveCorrectly)
{
	struct motion_profile p;
	float pos, vel, acc;

	// a simple profile with acceleration of 1, maximum speed of 2 and deceleration of 2
	motion_profile_init(&p, 1, 2, 2);
	// start at distance 3, velocity 0
	// end at distance 12, velocity 0
	motion_profile_plan_move(&p, 12, 0, 3, 0);
	// check position, velocity, acceleration at t == 1
	motion_profile_get_pva(&p, 1, &pos, &vel, &acc);
	ASSERT_FLOAT_EQ(11.5, pos);
	ASSERT_FLOAT_EQ(-1, vel);
	ASSERT_FLOAT_EQ(-1, acc);

	motion_profile_get_pva(&p, 2, &pos, &vel, &acc);
	ASSERT_FLOAT_EQ(10, pos);
	ASSERT_FLOAT_EQ(-2, vel);
	ASSERT_FLOAT_EQ(0, acc);

	ASSERT_FLOAT_EQ(6, motion_profile_get_traversal_time(&p));

	// check that we are stopped at the end
	int state =
		motion_profile_get_pva(&p, motion_profile_get_traversal_time(&p), &pos, &vel, &acc);
	ASSERT_FLOAT_EQ(3, pos);
	ASSERT_FLOAT_EQ(0, vel);
	ASSERT_FLOAT_EQ(0, acc);

	ASSERT_EQ(state, MOTION_PROFILE_COMPLETED);
}

TEST(MotionProfile, ShouldPlanTriangularMoveCorrectly)
{
	// a triangular move is one that never reaches maximum velocity
	struct motion_profile p;
	float pos, vel, acc;

	// a simple profile with acceleration of 1, maximum speed of 2 and deceleration of 2
	motion_profile_init(&p, 2, 8, 2);
	// start at distance 3, velocity 0
	// end at distance 12, velocity 0
	motion_profile_plan_move(&p, 3, 0, 5, 0);
	// check position, velocity, acceleration at t == 1
	motion_profile_get_pva(&p, 1, &pos, &vel, &acc);
	ASSERT_FLOAT_EQ(4, pos);
	ASSERT_FLOAT_EQ(2, vel);
	ASSERT_FLOAT_EQ(-2, acc);

	motion_profile_get_pva(&p, 2, &pos, &vel, &acc);
	ASSERT_FLOAT_EQ(5, pos);
	ASSERT_FLOAT_EQ(0, vel);
	ASSERT_FLOAT_EQ(0, acc);

	ASSERT_FLOAT_EQ(2, motion_profile_get_traversal_time(&p));
}
