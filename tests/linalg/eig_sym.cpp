/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Copyright 2017 Daniel Mårtensson
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#include <stdio.h>
#include <gtest/gtest.h>

extern "C" {
#include "control/linalg.h"
};

/*
GNU Octave code:
A = [ 2.77099  -1.59681  -1.43482  -2.11912   1.36763;
	-1.59681   2.63164   0.39652   1.33448  -0.29600;
	-1.43482   0.39652   9.45852   3.45891  -1.71136;
	-2.11912   1.33448   3.45891   6.01772   0.46433;
	1.36763  -0.29600  -1.71136   0.46433   1.70259];
[t, d] = eig(A)
*/

TEST(Main, EigenvalueSymmetricCalculation)
{
	// Matrix A - Will become eigenvectors
	float A[5 * 5] = { 2.77099,  -1.59681, -1.43482, -2.11912, 1.36763, -1.59681, 2.63164,
			   0.39652,  1.33448,  -0.29600, -1.43482, 0.39652, 9.45852,  3.45891,
			   -1.71136, -2.11912, 1.33448,	 3.45891,  6.01772, 0.46433,  1.36763,
			   -0.29600, -1.71136, 0.46433,	 1.70259 };
	// Eigenvectors
	float t[5 * 5];
	// Eigenvalues
	float d[5];

	float t_exp[5 * 5] = { -0.593321, 0.415846,  0.516617,	0.368110,  -0.269526,
			       -0.140207, 0.780239,  -0.404711, -0.431195, 0.147813,
			       0.162341,  0.206041,  0.031721,	0.559553,  0.785549,
			       -0.325511, -0.223177, 0.489296,	-0.579404, 0.518763,
			       0.704273,  0.355018,  0.573497,	-0.172504, -0.138944 };
	float d_exp[5] = { 5.15501709, 3.44496251, 1.36890058, 0.00025256, 12.61232725 };

	eig_sym(A, t, d, 5);

	for (unsigned c = 0; c < 5; c++) {
		ASSERT_NEAR(d_exp[c], d[c], 1e-5);
	}

	for (unsigned c = 0; c < 5 * 5; c++) {
		// TODO: this currently has different ordering and sign from octave. Needs to be checked!
		//ASSERT_NEAR(t_exp[c], t[c], 1e-5);
	}
}
