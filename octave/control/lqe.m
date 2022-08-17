% SPDX-License-Identifier: MIT
% Compute the LQE kalman gain matrix with the weighing matricies Q and R and state space model
% Input: SS, Q, R
% Example 1: [K] = lqe(sys, Q, R)
% Author: Daniel Mårtensson, October 2017

function [K] = lqe(varargin)
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
		% Change matrecies due to the Kalman Duality!
		sys.A = sys.A'; % Transpose
		sys.B = sys.C'; % Transpose
		K = (lqr(sys, Q, R))';
	elseif(strcmp(type, 'TF' ))
		disp('Only state space models only')
	else
		error('This is not TF or SS');
	end
end
