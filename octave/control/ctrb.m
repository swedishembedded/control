% SPDX-License-Identifier: MIT
% Generates the controllability matrix of a state space model
% Input: sys, n(optinal)
% Example 1: [Cs] = ctrb(sys)
% Example 1: [Cs] = ctrb(sys, n)
% Author: Daniel Mårtensson, Oktober 2017

function [Cs] = ctrb(varargin)
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

		% Check if we got variable n for minimal realization
		if(length(varargin) > 1)
			n = varargin{2};
		else
			n = size(sys.A, 1); % Else, we only check the dimension of A
		end

		% Compute the controllability matrix now!
		Cs = [];
		for i = 0:(n-1)
			Cs = [Cs sys.A^i*sys.B];
		end
	elseif(strcmp(type, 'TF' ))
		% Get TF
		error('Model must be a state space model');
	else
		error('This is not TF or SS');
	end
end
