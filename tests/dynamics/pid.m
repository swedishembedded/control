% SPDX-License-Identifier: MIT
% Copyright 2022 Martin Schröder <info@swedishembedded.com>
% Consulting: https://swedishembedded.com/go
% Training: https://swedishembedded.com/tag/training

pkg load control
pkg load symbolic

output_precision(9);

Ts = 0.1

function [p, i, d] = pid_imc(aggr, process_gain, time_constant, dead_time)
    tc = max((0.1 * aggr) * time_constant, (0.8 * aggr) * dead_time);
    kc = (1.0/process_gain)*((time_constant + 0.5 * dead_time)/(tc + 0.5 * dead_time));
    ti = time_constant + 0.5 * dead_time;
    td = (time_constant * dead_time)/(2.0 * time_constant + dead_time);

    p = kc;
    i = kc / ti;
    d = kc * td;
end

s = tf('s');
z = tf('z', Ts);

% System dynamics
G = 4/(0.4*s + 1);
Gd = ss(c2d(G, Ts))
Gd.a
Gd.b
Gd.c
Gd.d
% calculate ontinuous time gains
[Kp, Ki, Kd] = pid_imc(0.2, 4, 0.4, 0)
C = Kp + Kd * s + Ki / s

% convert into discrete time
Cd = c2d(C, Ts)
sys_Cd = ss(Cd)
sys_Cd.b
sys_Cd.d
