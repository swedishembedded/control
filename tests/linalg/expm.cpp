#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/linalg.h"
};

TEST(Main, MatrixExponential)
{
	float A[2 * 2] = { 0.798231, 0.191700, -0.575101, 0.031430 };
	float E_exp[2 * 2] = { 2.12609, 0.29204, -0.87611, 0.95795 };

	expm(A, A, 2);

	for (unsigned c = 0; c < 4; c++) {
		ASSERT_NEAR(E_exp[c], A[c], 1e-5);
	}
}
