% SPDX-License-Identifier: MIT
% Generates the observability matrix of a state space model
% Input: sys, n(optinal)
% Example 1: Or = obsv(sys)
% Example 2: Or = obsv(sys, n)
% Author: Daniel Mårtensson, 2017 Oktober

function [Or] = obsv(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get SS
		sys = varargin{1};

		% If we specify n, that means we are using minreal.m
		if(length(varargin) > 1)
			n = varargin{2};
		else
			n = size(sys.A, 1); % Else, we only check the dimension of A
		end

		% Compute the observability matrix now!
		Or = [];
		for i = 0:(n-1)
			Or = [Or; sys.C*sys.A^i];
		end

	elseif(strcmp(type, 'TF' ))
		% Get TF
		error('Model must be a state space model');
	else
		error('This is not TF or SS');
	end
end
