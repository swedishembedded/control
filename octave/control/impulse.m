% SPDX-License-Identifier: MIT
% Do a impulse response of a transfer function or a state space model
% Input: G, sys, t(optional)
% Example 1: [y,t,x] = impulse(G, t)
% Example 2: [y,t,x] = impulse(G)
% Example 3: [y,t,x] = impulse(sys, t)
% Author: Daniel Mårtensson, September 2017

function [y,t,X] = impulse(varargin)
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
		u = zeros(size(varargin{1}.B, 2), length(t)); % Creates 1 1 1 1 1 1 1
		u(:,1) = ones(size(u,1), 1);
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

		% Call impulse
		[y,t,X] = impulse(sys,t);
	else
		error('Not a state space model or a transfer function')
	end
end
