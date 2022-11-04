/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include <math.h>
#include "control/motor.h"
};

TEST(ParkTransform, Forward)
{
	for (float angle = 0; angle < 2 * M_PI; angle += M_PI / 18.f) {
		float abc[3] = { cosf(angle), cosf(angle - 2.f * M_PI / 3.f),
				 cosf(angle + 2.f * M_PI / 3.f) };
		float xyz[3] = { 0 };
		float dqz[3] = { 0 };

		clarke(xyz, abc);
		park(dqz, xyz, angle);

		// the vectors should be the same length
		float n_abc = sqrtf(abc[0] * abc[0] + abc[1] * abc[1] + abc[2] * abc[2]);
		float n_xyz = sqrtf(dqz[0] * dqz[0] + dqz[1] * dqz[1] + dqz[2] * dqz[2]);
		EXPECT_NEAR(n_abc, n_xyz, 1e-6);
	}
}

TEST(ParkTransform, Reverse)
{
	for (float angle = 0; angle < 2 * M_PI; angle += M_PI / 18.f) {
		float abc[3] = { cosf(angle), cosf(angle - 2.f * M_PI / 3.f),
				 cosf(angle + 2.f * M_PI / 3.f) };
		float xyz[3] = { 0 };
		float dqz[3] = { 0 };
		float i_xyz[3] = { 0 };
		float i_abc[3] = { 0 };

		clarke(xyz, abc);
		park(dqz, xyz, angle);
		inv_park(i_xyz, dqz, angle);
		inv_clarke(i_abc, i_xyz);

		EXPECT_NEAR(abc[0], i_abc[0], 1e-6);
		EXPECT_NEAR(abc[1], i_abc[1], 1e-6);
		EXPECT_NEAR(abc[2], i_abc[2], 1e-6);
	}
}
