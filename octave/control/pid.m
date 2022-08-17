% SPDX-License-Identifier: MIT
% Generates the parallel PID controller as a transfer function
% Input: Kp, Ti(Integrator, Optinal), Td(Derivative, Optional), Tf(Low pass
% filter, Optinal), Ts(Sample time, Optinal)
% Example 1: [Gpid] = pid(Kp, Ti, Td, Tf, Ts)
% Author: Daniel Mårtensson, Oktober 2017

function [Gpid] = pid(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing parameters')
	end

	% Input the parameters
	if(length(varargin) >= 1)
		Kp = tf(varargin{1}, 1);
		Gpid = Kp;
	else
		error('Need to have at least Kp');
	end

	% Integrator
	if(length(varargin) >= 2)
		Ki = tf(varargin{2}, [1 0]);
		Gpid = parallel(Kp, Ki);
	else
		Ti = 0;
	end

	% Derivative
	if(length(varargin) >= 3)
		Td = varargin{3};
	else
		Td = 0;
	end

	% Low pass filter for derivative
	if(length(varargin) >= 4)
		Tf = varargin{4};
		Kd = tf([Td 0], [Tf 1]);
	else
		Tf = 0;
		Kd = tf([Td 0], [1]);
	end

	% Sampling time
	if(length(varargin) >= 5)
		Ts = varargin{5};
	else
		Ts = 0;
	end

	% Build the PID
	% Check if derivative was 0
	if Td > 0
		Gpid = parallel(Gpid, Kd);
	end

	% Else - Just return Gpid as it is

	% Convert to discrete if needed
	if(Ts > 0)
		Gpid = c2d(Gpid, Ts);
	end

end
