// SPDX-License-Identifier: MIT
/**
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/motor.h"

#include <math.h>

void clarke(float *xyz, const float *const abc)
{
	xyz[0] = (2 * abc[0] - abc[1] - abc[2]) * (1.f / sqrtf(6.f));
	xyz[1] = (abc[1] - abc[2]) * (1.f / sqrtf(2.f));
	xyz[2] = (abc[0] + abc[1] + abc[2]) * (1.f / sqrtf(3.f));
}

void inv_clarke(float *abc, const float *const xyz)
{
	abc[0] = (1.f / sqrtf(3.f)) * xyz[2];
	abc[1] = abc[0] - (1.f / sqrtf(6.f)) * xyz[0];
	abc[2] = abc[1] - (1.f / sqrtf(2.f)) * xyz[1];
	abc[1] += (1.f / sqrtf(2.f)) * xyz[1];
	abc[0] += (sqrtf(2.f / 3.f)) * xyz[0];
}
