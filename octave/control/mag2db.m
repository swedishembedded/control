% SPDX-License-Identifier: MIT
% Convert magnitude to decibel
% Input mag
% Example 1: [dB] = mag2db(mag)
% Author: Daniel Mårtensson 2017, Oktober
function [dB] = mag2db(mag)
	mag(mag < 0) = NaN;
	dB = 20*log10(mag);
end
