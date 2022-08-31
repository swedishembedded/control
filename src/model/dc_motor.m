% SPDX-License-Identifier: MIT
% Copyright 2022 Martin Schröder <info@swedishembedded.com>
% Consulting: https://swedishembedded.com/go
% Training: https://swedishembedded.com/tag/training

dir = fileparts(mfilename('fullpath'));
path(strcat(dir, "/../../octave/"), path);

pkg load control
pkg load symbolic

# Step 1: start with a continuous time state space model
syms s J b K R L z Ts
A = [-b/J   K/J;
    -K/L   -R/L];
B = [0;
    1/L];
C = [1   0];
D = 0;
sys = ss(A, B, C, D);

# Step 2: transform into a continuous time transfer function
Hz = tf(sys, Ts)

# Step 4: extract the difference equation
[N, D] = numden(Hz)

# Step 5: generate C code
ccode(Hz)
