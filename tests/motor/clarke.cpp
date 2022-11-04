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

TEST(ClarkeTransform, Forward)
{
	for (float angle = 0; angle < 2 * M_PI; angle += M_PI / 18.f) {
		float abc[3] = { cosf(angle), cosf(angle - 2.f * M_PI / 3.f),
				 cosf(angle + 2.f * M_PI / 3.f) };
		float xyz[3] = { 0 };

		clarke(xyz, abc);

		// the vectors should be the same length
		float n_abc = sqrtf(abc[0] * abc[0] + abc[1] * abc[1] + abc[2] * abc[2]);
		float n_xyz = sqrtf(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
		EXPECT_NEAR(n_abc, n_xyz, 1e-6);
	}
}

TEST(ClarkeTransform, Backward)
{
	for (float angle = 0; angle < 2 * M_PI; angle += M_PI / 18.f) {
		float abc[3] = { cosf(angle), cosf(angle - 2.f * M_PI / 3.f),
				 cosf(angle - 4.f * M_PI / 3.f) };
		float xyz[3] = { 0 };
		float abc_n[3] = { 0 };
		clarke(xyz, abc);
		inv_clarke(abc_n, xyz);
		EXPECT_NEAR(abc[0], abc_n[0], 1e-6);
		EXPECT_NEAR(abc[1], abc_n[1], 1e-6);
		EXPECT_NEAR(abc[2], abc_n[2], 1e-6);
	}
}
