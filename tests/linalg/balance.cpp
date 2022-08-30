#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/linalg.h"
};

TEST(Main, BalanceSquareMatrix)
{
	float A[] = { -0.1, 10.0, -1.0, -5.0 };
	float Abal[] = { -0.1, 2.5, -4.0, -5.0 };
	balance(A, 2);
	for (unsigned int c = 0; c < 4; c++) {
		ASSERT_FLOAT_EQ(Abal[c], A[c]);
	}
}
