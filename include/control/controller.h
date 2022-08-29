/* SPDX-License-Identifier: MIT */
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */
#include <stdbool.h>
#include <stdint.h>

struct pid {
	float x[3];
	float y[3];
	float Kp, Ki, Kd, d;
};

void pid_init(struct pid *self);
void pid_set_gains(struct pid *self, float Kp, float Ki, float Kd, float d);
float pid_step(struct pid *self, float e);

void mpc(float A[], float B[], float C[], float x[], float u[], float r[], uint8_t ADIM,
	 uint8_t YDIM, uint8_t RDIM, uint8_t HORIZON, uint8_t ITERATION_LIMIT,
	 bool has_integration);
void kalman(float A[], float B[], float C[], float K[], float u[], float x[], float y[],
	    uint8_t ADIM, uint8_t YDIM, uint8_t RDIM);
void lqi(float y[], float u[], float qi, float r[], float L[], float Li[], float x[], float xi[],
	 uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t ANTI_WINDUP);
void mrac(float limit, float gain, float y[], float u[], float r[], float I1[], float I2[],
	  uint8_t RDIM);
void theta2ss(float A[], float B[], float C[], float theta[], float K[], uint8_t ADIM, uint8_t NP,
	      uint8_t NZ, uint8_t NZE, bool integral_action);
bool stability(float A[], uint8_t ADIM);
void c2d(float A[], float B[], uint8_t ADIM, uint8_t RDIM, float sampleTime);
