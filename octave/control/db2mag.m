% SPDX-License-Identifier: MIT
% Convert decibel to magnitude
% Input dB
% Example 1: [mag] = db2mag(dB)
% Author: Daniel Mårtensson, Oktober 2017
function [mag] = db2mag (dB)
	mag = 10.^(dB/20);
end
