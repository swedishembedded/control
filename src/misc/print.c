// SPDX-License-Identifier: MIT
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#include "control/misc.h"

#include <stdio.h>

/*
 * Print matrix or vector - Just for error check
 */
void print(float A[], uint16_t row, uint16_t column)
{
	for (uint16_t i = 0; i < row; i++) {
		for (uint16_t j = 0; j < column; j++) {
			printf("%c%0.18f\t", (j) ? ',' : ' ', *(A++));
		}
		printf(";\n");
	}
	printf("\n");
}
