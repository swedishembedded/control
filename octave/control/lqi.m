% SPDX-License-Identifier: MIT
% Compute the LQI integral gain matrix control law L with the weighing matricies
% Q and R and state space model
% Input: SS, Q, R
% Example 1: [L, Li] = lqi(sys, Q, R)
% Example 2: [L, Li, sysi] = lqi(sys, Q, R)
% Author: Daniel Mårtensson, October 2017
% Update 2019-06-07: Returning the model wth integral action

function [L, Li, sys] = lqi(varargin)
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
		A = sys.A;
		B = sys.B;
		C = sys.C;
		D = sys.D;

		% Get info
		ny = size(C, 1); % Number outputs
		nu = size(B, 2); % Number inputs
		nx = size(A, 1); % Number states

		% Create the augmented state space model
		if(sys.sampleTime > 0)
			sys.A = [A zeros(nx, ny); -C ones(ny, ny)];
			sys.B = [B; -D];
			sys.C = [C zeros(ny, ny)];
			sys.D = D;
		else
			sys.A = [A zeros(nx, ny); -C zeros(ny, ny)];
			sys.B = [B; -D];
			sys.C = [C zeros(ny, ny)];
			sys.D = D;
		end

		% Get the LQR + LQI control law
		ControlLaw = lqr(sys, Q, R);
		% Important to have a negative sign!
		Li = -ControlLaw(:, (1 + size(C, 2)):end); % We add +1 beacuse we want -Li from [L -Li].
		L = ControlLaw(:, 1:size(C, 2));
	elseif(strcmp(type, 'TF' ))
		error('Model must be a state space model')
	else
		error('This is not TF or SS');
	end
end
