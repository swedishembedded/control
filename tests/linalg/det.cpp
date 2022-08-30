#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/linalg.h"
};

TEST(Main, Determinant)
{
	float A0[] = { 1, 2, 3, 4 };
	float A1[] = { 23, -1, -40, 50 };
	float A2[] = { 23 };

	ASSERT_FLOAT_EQ(-2, det(A0, 2));
	ASSERT_FLOAT_EQ(1110, det(A1, 2));
	ASSERT_FLOAT_EQ(23, det(A2, 1));
}
