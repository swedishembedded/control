% SPDX-License-Identifier: MIT
% Copyright 2022 Martin Schröder <info@swedishembedded.com>
% Consulting: https://swedishembedded.com/go
% Training: https://swedishembedded.com/tag/training

% Design of the pid controller
dir = fileparts(mfilename('fullpath'));
path(strcat(dir, "/../../octave/"), path);

pkg load symbolic

syms Kp Ki Kd d Ts z

G = Kp + Kd * ((1 - d)*(z - 1))/(z-d) + Ki / (z-1);
G = simplify(expand(G));
G = sym_tf(G, Ts);
[N, D] = numden(G)
ccode(G)
