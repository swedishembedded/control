% SPDX-License-Identifier: MIT
% Ramp a state space model or a transfer function
% Input: sys, G, t(optinal)
% Example 1: [y,t,x] = ramp(G, t)
% Example 2: [y,t,x] = ramp(G)
% Example 3: [y,t,x] = ramp(sys, t)
% Author: Daniel Mårtensson, 2017 September

function [y,t,X] = ramp(varargin)
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
		u = linspace(0,1, length(t));
		u = repmat(u, size(varargin{1}.B, 2), 1); % Creates 0 -> 1
		x0 = zeros(size(varargin{1}.A, 1), 1); % Assume x0 = [0; 0; 0; ..... ; 0]
		% Call lsim!
		[y,t,X] = lsim(varargin{1}, u, t, x0);
	elseif(strcmp(varargin{1}.type,'TF'))
		% TF to SS
		sys = tf2ss(varargin{1}, 'OCF');

		% Get time
		if(length(varargin) >= 2)
			t = varargin{2};
		else
			disp('Time assumed to be 10 seconds');
			t = 10;
		end

		% Call ramp
		[y,t,X] = ramp(sys,t);
	else
		error('Not a state space model or a transfer function')
	end
end
