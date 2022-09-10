/* SPDX-License-Identifier: MIT */
/**
 * Copyright 2019 Daniel Mårtensson <daniel.martensson100@outlook.com>
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/consulting
 * Simulation: https://swedishembedded.com/simulation
 * Training: https://swedishembedded.com/training
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_ITERATION_COUNT_SVD 30 // Maximum number of iterations for svd_jacobi_one_sided.c

#if !defined(CONSTRAIN_FLOAT)
#define CONSTRAIN_FLOAT(n, low, high) (((n) > (high)) ? (high) : (((n) < (low)) ? (low) : (n)))
#endif

/**
 * \brief Element-wise addition of two matrices
 * \details
 *   A[m*n]
 *
 *   B[m*n]
 *
 *   C[m*n]
 *
 *   C = A + B
 *
 * \param C Output matrix
 * \param A Input matrix A
 * \param B Input matrix B
 * \param row Number of rows
 * \param column Number of columns
 **/
void add(float *C, const float *const A, const float *const B, uint16_t row, uint16_t column);

/**
 * \brief Matrix inverse
 * \details
 *   Ai = A^-1
 *
 *   Notice that only square matrices are only allowed.
 *
 *   Finding inverse should only be used for last solution.
 *
 *   Finding inverse is very expensive. Better to solve Ax=b instead
 * \param Ai output matrix Ai
 * \param A Input matrix A[n*n]
 * \param row Number of rows and columns in A
 * \retval 0 Success
 * \retval -ENOTSUP Inverse not possible with given matrix
 **/
int inv(float *Ai, const float *const A, uint16_t row);

/**
 * \brief This solves Ax = b.
 * \details
 *   A need to be square and upper triangular.
 *
 *   A [m*n]
 *
 *   b [m]
 *
 *   m == n
 * \param A Input matrix A
 * \param x Output vector X
 * \param b Input matrix/vector b
 * \param column Number of rows and columns in A
 */
void linsolve_upper_triangular(const float *const A, float *x, const float *const b,
			       uint16_t column);
/**
 * \brief Turn A into transpose of A
 * \details
 *   At = A'
 * \param At Output matrix [n*m]
 * \param A Input matrix [m*n]
 * \param row Number of rows in A and columns in At
 * \param column Number of columns in A and rows in At
 **/
void tran(float *At, const float *const A, uint16_t row, uint16_t column);
/**
 * \brief C = A * B
 * \param C Ourput Matrix [row_a*column_b]
 * \param A Input matrix [row_a*column_a]
 * \param B Input matrix [column_a*column_b]
 * \param row_a Number of rows in A
 * \param column_a Number of columns in A
 * \param row_b Number of rows in B
 * \param column_b Number of columns in B (and rows in C)
 **/
int mul(float *C, const float *const A, const float *const B, uint16_t row_a, uint16_t column_a,
	uint16_t row_b, uint16_t column_b);
/**
 * \brief Singular Value Decomposition A = USV^T (Economy mode)
 * \details
 *   This uses Jacobi rotation method.
 *
 *   http://www.netlib.org/lapack/lawnspdf/lawn15.pdf
 *
 *   Use this SVD method if you have a square matrix A.
 *
 *   A [m*n] // This will be set to U
 *
 *   U [m*m]
 *
 *   S [n]
 *
 *   V [n*n]
 *
 *   n == m
 * \param A Input matrix [m*n]
 * \param row Number of rows in A
 * \param max_iterations Maximum number of iterations (10 is a good start)
 * \param U Output U matrix [m*m]
 * \param S Output S diagonal vector [n]
 * \param V Output V matrix [n*n]
 **/
void svd_jacobi_one_sided(const float *const A, uint16_t row, uint8_t max_iterations, float *U,
			  float *S, float *V);
/**
 * \brief Singular Value Decomposition A = USV^T (Economy mode)
 * \details
 *     This routine decomposes an m x n matrix A, with m >= n, into a product
 *     of the three matrices U, D, and V', i.e. A = UDV', where U is an m x n
 *     matrix whose columns are orthogonal, D is a n x n diagonal matrix, and
 *     V is an n x n orthogonal matrix.  V' denotes the transpose of V.  If
 *     m < n, then the procedure may be used for the matrix A'.  The singular
 *     values of A are the diagonal elements of the diagonal matrix D and
 *     correspond to the positive square roots of the eigenvalues of the
 *     matrix A'A.
 *
 *     This procedure programmed here is based on the method of Golub and
 *     Reinsch as given on pages 134 - 151 of the "Handbook for Automatic
 *     Computation vol II - Linear Algebra" edited by Wilkinson and Reinsch
 *     and published by Springer-Verlag, 1971.
 *
 *     The Golub and Reinsch's method for decomposing the matrix A into the
 *     product U, D, and V' is performed in three stages:
 *       Stage 1:  Decompose A into the product of three matrices U1, B, V1'
 *         A = U1 B V1' where B is a bidiagonal matrix, and U1, and V1 are a
 *         product of Householder transformations.
 *       Stage 2:  Use Given' transformations to reduce the bidiagonal matrix
 *         B into the product of the three matrices U2, D, V2'.  The singular
 *         value decomposition is then UDV'where U = U2 U1 and V' = V1' V2'.
 *       Stage 3:  Sort the matrix D in decreasing order of the singular
 *         values and interchange the columns of both U and V to reflect any
 *         change in the order of the singular values.
 *
 *   A [m*n]
 *
 *   U [m*n]
 *
 *   S [n]
 *
 *   V [n*n]
 *
 *   m = row
 *
 *   n = column
 *
 *   If m < n, then this procedure is for A'
 * \param A Input matrix [m * n]
 * \param U Output U Matrix [m * m]
 * \param S Output S Diagonal Vector [n]
 * \param V Output V Matrix [n * n]
 * \param row Number of rows in A (m)
 * \param column Number of columns in A (n)
 * \retval 0 Success
 * \retval -ENOTSUP SVD decomposition not supported for given matrix
 **/
int svd_golub_reinsch(const float *const A, uint16_t row, uint16_t column, float *U, float *S,
		      float *V);
/**
 * \brief Solves discrete Lyapunov equation
 * \details
 *   Solves A * P * A' - P + Q = 0
 *
 *   A, P, Q need to be square and Q need to be positive and symmetric
 *
 *   A [m*n]
 *
 *   Q [m*n]
 *
 *   P [m*n]
 *
 *   n == m
 * \param A input matrix A
 * \param P Solution to the Lyapunov equation
 * \param Q input matrix Q
 * \param row size of A P and Q (square)
 **/
void dlyap(const float *const A, float *P, const float *const Q, uint16_t row);

/**
 * \brief Householder QR-decomposition
 * \details
 *   A [m*n]
 *
 *   Q [m*m]
 *
 *   R [m*n]
 * \param A Input matrix to decompose
 * \param Q Output Q matrix
 * \param R Output R matrix
 * \param row_a Rows in A
 * \param column_a Columns in A
 * \param only_compute_R If set, only R is computed
 * \retval 0 Success
 * \retval -ENOTSUP QR decomposition not supported for this input matrix
 **/
int qr(const float *const A, float *Q, float *R, uint16_t row_a, uint16_t column_a,
       bool only_compute_R);
/**
 * \brief Solve Ax=b with QR decomposition
 * \details
 *   A[m*n]
 *
 *   b[m]
 *
 *   x[n]
 * \param A Input matrix A
 * \param x Output vector x
 * \param b Input vector b
 * \param row Number of rows in A
 * \param column Number of columns in A
 **/
void linsolve_qr(const float *const A, float *x, const float *const b, uint16_t row,
		 uint16_t column);
/**
 * \brief Solve with forward substitution. This can be used with Cholesky decomposition
 * \details
 *   A [m*n] need to be lower triangular and square
 *
 *   b [m]
 *
 *   x [n]
 *
 *   n == m
 * \param A Input matrix
 * \param x output vector
 * \param b Input vector
 * \param row Number of tows and columns
 **/
void linsolve_lower_triangular(const float *const A, float *x, const float *const b, uint16_t row);

/**
 * \brief Solve for markov parameters from output y and input u
 * \details
 * This is just a simple linear solve Ax = b where A is lower toeplitz
 * triangular shape and x is a vector of g.
 * So the formula is u * g = y, and we want to solve g = y/u
 * u is a vector, but it's interpreted as lower triangular A:
 * [u0  0  0  0  0 0 0]  [g0] [y0]
 * [u1 u0 0 0 0 0 0]     [g1] [y1]
 * [u2 u1 u0 0 0 0 0]    [g2] [y2]
 * [u3 u2 u1 u0 0 0 0]   [g3] [y3]
 * [u4 u3 u2 u1 u0 0 0]  [g4] [y4]
 * [u5 u4 u3 u2 u1 u0 0] [g5] [y5]
 * [.. u5 u4 u3 u2 u1 u0][g6] [y6]
 * [un .. .. .. .. .. ..][gn] [yn]
 *
 * Where g0 = y0/u0 and g1 = (y1 - u1*g0)/u0 etc..
 * \param g Markov parameters [m * n]
 * \param y Measurement signal [m * n]
 * \param u Input signal [m * n]
 * \param m Number of rows
 * \param n Number of columns
 **/
void linsolve_markov(float *g, const float *const y, const float *const u, uint16_t m, uint16_t n);

/**
 * \brief Do LU-decomposition with partial pivoting
 * \details
 *    A [m*n]
 *
 *    LU [m*n]
 *
 *    P [n]
 *
 *    n == m
 * \retval 0 Success
 * \retval -ENOTSUP No decomposition exists for A
 **/
int lup(const float *const A, float *LU, uint8_t *P, uint16_t row);
/**
 * \brief Calculate determinant of a square matrix A
 * \param A Square matrix A
 * \param row Number of rows and columns in A
 * \returns determinant of A
 **/
float det(const float *const A, uint16_t row);
/**
 * \brief Solves Ax=b with LUP-decomposition
 * \details
 *   A [m*n]
 *
 *   x [n]
 *
 *   b [m]
 *
 *   n == m
 * \param A Input matrix A
 * \param x Output vector x
 * \param b Input vector b
 * \param row Number of columns and rows in A
 * \retval 0 Success
 * \retval -ENOTSUP Decomposition not supported for this matrix
 **/
int linsolve_lup(const float *const A, float *x, const float *const b, uint16_t row);
/**
 * \brief Perform lower triangular Cholesky decomposition of matrix A
 * \details
 *    L = chol(A) => A = L * L'
 * \param A Symmetric positive definite matrix
 * \param L Output L matrix so that A = L*L^T (transposed)
 * \param row Number of rows and columns in A
 **/
void chol(const float *const A, float *L, uint16_t row);
/**
 * \brief Perform cholesky update on a lower triangular cholesky decomposition
 * \details
 *   When you have L = chol(A) and you need to compute chol(A + x * x'),
 *   it is faster to compute cholupdate(L, x) instead.
 * \param L Lower triangular cholesky decomposition matrix.
 * \param x Vector to update with
 * \param row Number of rows and columns in L
 * \param rank_one_update Whether to perform a Rank1 update or downdate
 **/
void cholupdate(float *L, const float *const x, uint16_t row, bool rank_one_update);

/**
 * \brief Solves Ax=b with Cholesky decomposition
 * \details
 *   A [m*n] (symmetric and positive definite)
 *
 *   x [n]
 *
 *   b [m]
 *
 *   n == m
 * \param A Input matrix
 * \param x Unknown vector
 * \param b Right hand side
 * \param row Number of rows in A
 **/
void linsolve_chol(const float *const A, float *x, const float *const b, uint16_t row);

/**
 * \brief Pseudo inverse by using Singular Value Decomposition
 * \details
 *   A [m*n]
 *
 *   m >= n
 *
 *   Ai = V*inv(S)*U'
 * \param Ai Result matrix (can be same as input matrix)
 * \param A Input matrix
 * \param row Number of rows in A
 * \param column Number of columns in A
 **/
void pinv(float *Ai, const float *const A, uint16_t row, uint16_t column);
/**
 * \brief Create hankel matrix of vector V. Step is just the shift. Normaly set this to 0.
 * \details
 *   V [m*n]
 *
 *   H [m*n]
 *
 *   shift >= 0 // Set this to 0 if you want a normal hankel matrix
 * \param V Input vector
 * \param H Output Hankel matrix
 * \param row_v Number of rows in V
 * \param column_v Number of columns in V
 * \param row_h Number of rows in H
 * \param column_h Number of columns in H
 * \param shift Shift parameter
 * \retval 0 Success
 * \retval -EINVAL Invalid arguments
 **/
int hankel(const float *const V, float *H, uint16_t row_v, uint16_t column_v, uint16_t row_h,
	   uint16_t column_h, uint16_t shift);
/**
 * \brief Balance a matrix A to reduce numerical errors in future calculations
 * \param A Square matrix
 * \param row Number of rows and columns in A
 **/
void balance(float A[], uint16_t row);
/**
 * \brief Find eigenvalues from a general matrix
 * \details
 *   A [m*n]
 *
 *   wr [m]
 *
 *   wi [m]
 *
 *   n == m
 * \param A Input matrix
 * \param wr Real eigenvalues
 * \param wi Imaginary eigenvalues
 * \param row Number of rows in A
 **/
void eig(const float *const A, float *wr, float *wi, uint16_t row);
/**
 * \brief Compute eigenvalues and eigenvectors from a symmetrical square matrix A
 * \details
 *   Notice that a square symmetrical matrix can never have complex eigenvalues and eigenvalues!
 *
 *   A [m*n]
 *
 *   n == m
 *
 *   A^T = A
 *
 *   d [m] // Eigenvalues
 *
 *   A will become eigenvectors!
 * \param A Square symmetric input matrix
 * \param ev Eigenvector square matrix [row*row]
 * \param d Eigenvalues of A
 * \param row Number of rows in A
 **/
void eig_sym(const float *const A, float *ev, float *d, uint16_t row);
/**
 * \brief Sum elements of a matrix
 * \details
 *   A[m*n]
 *
 *   l = 1 sum on rows -> First row is the result
 *
 *   l = 2 sum on columns -> First column is the result
 * \param Ar Result matrix [m*n]
 * \param A Input matrix
 * \param row Number of rows
 * \param column Number of columns
 * \param l Type of sum
 **/
void sum(float *Ar, const float *const A, uint16_t row, uint16_t column, uint8_t l);
/**
 * \brief Find the norm of A
 * \details
 *   A[m*n]
 *
 *   l = 1 = L1-norm
 *
 *   l = 2 = L2-norm
 * \param A Input matrix A
 * \param row Number of rows in A
 * \param column Number of columns in A
 * \param l Whether to return L1 or L2 norm
 **/
float norm(const float *const A, uint16_t row, uint16_t column, uint8_t l);
/**
 * \brief Find matrix exponential, return A as A = expm(A)
 * \details
 *   A[m*n]
 *
 *   m == n
 * \param A Input matrix
 * \param exp Output matrix
 * \param row Size of input matrix (must be square)
 **/
void expm(const float *const A, float *exp, uint16_t row);
void nonlinsolve(void (*nonlinear_equation_system)(float[], float[], float[]), float b[], float x[],
		 uint8_t elements, float alpha, float max_value, float min_value,
		 bool random_guess_active);
/**
 * \brief Solve Ax = b using gaussian elemination
 * \details
 *   A[m*n]
 *
 *   b[m]
 *
 *   x[n]
 *
 *   If alpha = 0:
 *     m == n
 *
 *   If alpha > 0: (Tikhonov regularization is used)
 *     m =/= n
 * \param A Input matrix A
 * \param x Output solution vector x
 * \param b Input matrix/vector b
 * \param row Number of rows in A and b
 * \param column Number of columns in A and x
 * \param alpha Alpha parameter (if 0 then A must be square!)
 **/
void linsolve_gauss(const float *const A, float *x, const float *const b, uint16_t row,
		    uint16_t column, float alpha);
