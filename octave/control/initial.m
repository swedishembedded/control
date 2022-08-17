% SPDX-License-Identifier: MIT
% Simulate a transfer function or state space model with initial state vector x0
% and time constant t, not time vector
% Input: sys, t, x0
% Example 1: [y,t,x] = initial(sys, t, x0)
% Author: Daniel Mårtensson, September 2017

function [y,t,X] = initial(varargin)
	% Check if there is some input arguments or it's not a model
	if(isempty(varargin{1}))
		error ('Missing input')
	end

	% Check if there is a model
	if(strcmp(varargin{1}.type,'SS'))

		% Get time
		if(length(varargin) >= 2)
			t = varargin{2};
		else
			disp('Time assumed to be 10 seconds');
			t = 10;
		end

		% Get sample time to compute the new time vector
		sampleTime = varargin{1}.sampleTime;
		if(sampleTime > 0)
			t = 0:sampleTime:t;
		else
			t = 0:0.01:t; % Sample time assumed to be 0.01
		end

		% Multiple signals...or not!
		u = zeros(size(varargin{1}.B, 2), length(t)); % Creates 0 0 0 0 0 0 0

		% Get initial conditions
		if(length(varargin) >= 3)
			x0 = varargin{3};
			x0 = x0(:); % Turn them into a vector
			if(size(varargin{1}.A, 1) ~= size(x0, 1))
				error('The initial conditions vector has not the same row length as matrix A')
			end
		else
			error('Missing initial conditions');
		end

		% Call lsim!
		[y,t,X] = lsim(varargin{1}, u, t, x0);
	elseif(strcmp(varargin{1}.type,'TF'))
		error('Only for state space models')
	else
		error('Not a state space model or a transfer function')
	end
end
