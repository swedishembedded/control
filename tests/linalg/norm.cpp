#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/linalg.h"
};

TEST(Main, Norm)
{
	float A[2 * 2] = { 1, 2, 3, 4 };

	EXPECT_NEAR(6, norm(A, 2, 2, 1), 1e-4);
	EXPECT_NEAR(5.4650, norm(A, 2, 2, 2), 1e-4);
}
