% SPDX-License-Identifier: MIT
% Generates the balanced recuded state space model of a balanced state space model
% Remove states with the tolerance of Hankel singular values
% Input: SS, tol
% Example 1: model = modred(sys, tol)
% Author: Daniel Mårtensson, 2017 October

function [model] = modred(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing balanced state space model')
	end

	if(length(varargin) >= 2)
		tol = varargin{2};
	else
		error('Missing tolerance for the hankel signular values');
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get sys
		sys = varargin{1};
		A = sys.A;
		B = sys.B;
		C = sys.C;
		D = sys.D;
		sampleTime = sys.sampleTime;
		delay = sys.delay;

		% Get hankel singular values
		hsv = diag(hsvd(sys));
		hsv = hsv(hsv >= tol);
		n = size(hsv, 1); % Dimension states
		A = A(1:n, 1:n);
		B = B(1:n, :);
		C = C(:,1:n);

		% Create the state space model
		model = ss(delay, A, B, C, D);
		model.sampleTime = sampleTime;
	elseif(strcmp(type, 'TF' ))
		error('Model must be a state space model')
	else
		error('This is not TF or SS');
	end
end
