// SPDX-License-Identifier: MIT
/**
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/motor.h"

#include <math.h>

void park(float *dqz, const float *const xyz, const float angle)
{
	float co = cosf(angle);
	float si = sinf(angle);

	dqz[0] = co * xyz[0] + si * xyz[1];
	dqz[1] = co * xyz[1] - si * xyz[0];
	dqz[2] = xyz[2];
}

void inv_park(float *xyz, const float *const dqz, const float angle)
{
	float co = cosf(angle);
	float si = sinf(angle);

	xyz[0] = co * dqz[0] - si * dqz[1];
	xyz[1] = si * dqz[0] + co * dqz[1];
	xyz[2] = dqz[2];
}
