% SPDX-License-Identifier: MIT
% Generates the serial PID controller as a transfer function
% Input: Kp, Ti(optinal), Td(optional), b(optinal), Ts(optinal)
% Example 1: [Gpipd] = pipd(Kp, Ti, Td, b, Ts)
% Author: Daniel Mårtensson, Oktober 2017

function [Gpipd] = pipd(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing parameters')
	end

	% Input the parameters
	if(length(varargin) >= 1)
		Kp = varargin{1};
	else
		error('Need to have at least Kp');
	end

	% Integrator
	if(length(varargin) >= 2)
		Ti = varargin{2};
	else
		Ti = 0;
	end

	% Derivative
	if(length(varargin) >= 3)
		Td = varargin{3};
	else
		Td = 0;
	end

	% High pass filter
	if(length(varargin) >= 4)
		b = varargin{4};
	else
		b = 0;
	end

	% Sampling time
	if(length(varargin) >= 5)
		Ts = varargin{5};
	else
		Ts = 0;
	end

	% Build the PIPD
	Gpipd = tf([Kp*Ti*Td Kp*(Ti+Td) Kp],[Ti*Td/b Ti 0]);

	% Convert to discrete if needed
	if(Ts > 0)
		Gpipd = c2d(Gpipd, Ts);
	end
end
