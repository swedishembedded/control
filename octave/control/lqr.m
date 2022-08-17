% SPDX-License-Identifier: MIT
% Compute the LQR gain matrix control law L with the weighing matricies Q and R
% and state space model
% Input: sys, Q, R
% Example 1: [L] = lqr(sys, Q, R)
% Author: Daniel Mårtensson, October 2017

function [L] = lqr(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing model')
	end

	% Get Q R
	if(length(varargin) >= 2)
		Q = varargin{2};
		R = varargin{3};
	else
		error('Missing Q or R');
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get model
		sys = varargin{1};
		% Solve the Algebraic Riccati Equation
		X = are(sys, Q, R);
		% Return the control law L
		if(sys.sampleTime > 0)
			L = inv(sys.B'*X*sys.B + R)*(sys.B'*X*sys.A);
		else
			L = inv(R)*sys.B'*X;
		end
	elseif(strcmp(type, 'TF' ))
		error('Model must be a state space model')
	else
		error('This is not TF or SS');
	end
end
