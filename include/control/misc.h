/* SPDX-License-Identifier: MIT */
/*
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#pragma once

#include <stdint.h>

/**
 * \brief Concatenate two matrices
 * \details
 * C = [A; B] (vertical == 1)
 * C = [A, B] (vertical == 0)
 * \param C Output matrix
 * \param A Input matrix A
 * \param B Input matrix B
 * \param vertical Specifies if vertical concatenation is needed
 * \param row_a Number of rows in A
 * \param column_a Number of columns in A
 * \param row_b Number of rows in B
 * \param column_b Number of columns in B
 * \param row_c Number of rows in C
 * \param column_c Number of columns in C
 **/
void cat(float *C, const float *const A, const float *const B, int vertical, uint16_t row_a,
	 uint16_t column_a, uint16_t row_b, uint16_t column_b, uint16_t row_c, uint16_t column_c);
/**
 * \brief Constrain input between two limits
 * \param input Input value
 * \param lower_limit Lower limit value
 * \param upper_limit Upper limit value
 * \returns input constrained
 **/
float constrain_float(float input, float lower_limit, float upper_limit);

/**
 * \brief Constrain a vector between limits
 * \details
 * Upper limit must be above lower limit
 * \param Iout Output vector of values
 * \param Iin Vector of values
 * \param dim Size of vector
 * \param lower Lower limit
 * \param upper Upper limit
 **/
void constrain(float *Iout, const float *const Iin, uint16_t dim, float lower, float upper);

/**
 * \brief Cut area out of A and place it into B
 * \details
 * Cut a matrix A with size row x columns into a matrix B with size (stop_row -
 * start_row + 1) x (stop_column - start_column + 1).  Remember! Indexing start
 * and stop are from zero!
 *
 * Example:
 * If you have a matrix A 5 x 6 and you want to cut the values from A to matrix
 * B with size 3 x 3 and you want to start at 0,0 and end at 3,3 Code: cut(A, B,
 * 5, 6, 0, 0, 2, 2); // Because indexing from zero
 * \param B Output matrix
 * \param A Input matrix
 * \param row Number of rows in input matrix
 * \param column Number of columns in input matrix
 * \param start_row Start position row
 * \param start_column Start position column
 * \param row_b Number of rows in B
 * \param column_b Number of columns in B
 **/
void cut(float *B, const float *const A, uint16_t row, uint16_t column, uint16_t start_row,
	 uint16_t start_column, uint16_t row_b, uint16_t column_b);
/**
 * \brief Insert matrix A, size row x column inside matrix B, row x column.
 * \details
 * The variables startRow_b and startColumn_b describe which row and column we
 * should use to insert first element of A into B.
 *
 * Elements in B where A is placed are overwritten.
 *
 * Notice that start positions are indexed from zero
 *
 * B = insert(B, A)
 * \param B Output matrix
 * \param A Input matrix
 * \param row_a Number of rows in A
 * \param column_a Number of columns in A
 * \param column_b Number of columns in B (needed to know where to insert)
 * \param startRow_b Start row position in B
 * \param startColumn_b Start column position in B
 **/
void insert(float *B, const float *const A, uint16_t row_a, uint16_t column_a, uint16_t column_b,
	    uint16_t startRow_b, uint16_t startColumn_b);
/**
 * \brief Print out a matrix
 * \param A Matrix to print
 * \param row Number of rows in A
 * \param column Number of columns in A
 **/
void print(float A[], uint16_t row, uint16_t column);
/**
 * \brief Return sign (-1 or 1)
 * \param number Input number
 * \retval -1 If sign is negative
 * \retval 1 If positive
 * \retval 0 If number is zero
 **/
float sign(float number);
/**
 * \brief Compute Gaussian Distribution
 * \details
 * Make sure that you initialize your random number generator to a seed before
 * startup!
 * \param x Vector: x[length]
 * \param length length of vector x
 * \param mu Mean of the distribution
 * \param sigma Standard deviation
 **/
void randn(float *x, uint16_t length, float mu, float sigma);
/**
 * \brief Compute the mean of vector x
 * \param x x[L] Vector with values
 * \param length L = Length of vector x
 * \returns mean value of the elements in x
 **/
float mean(const float *const x, uint16_t length);
/**
 * \brief Compute Standard deviation
 * \param x x[L] Vector with values
 * \param length Length of vector x
 * \returns Standard deviation of elments in x
 **/
float stddev(const float *const x, uint16_t length);
