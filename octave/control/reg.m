% SPDX-License-Identifier: MIT
% Generates the state feedback controler with the control law L and with integral action law Li
% Input: sys, L, Li(optional), Kr(optional)
% Example 1: [regsys] = reg(sys, L)
% Example 2: [regsys] = reg(sys, L, Li)
% Example 3: [regsys] = reg(sys, L, Li)
% Author: Daniel Mårtensson, November 2017

function [regsys] = reg(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end
	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))

		% Get the control law L;
		if(length(varargin) >= 2)
			L = varargin{2};
		else
			error('Missing the control law L');
		end

		% Get the integral control law Li
		if(length(varargin) >= 3)
			Li = varargin{3};
		end

		% Check if what feedback controller you should use
		regulatorNumber = length(varargin);

		% Get A, B, C, D matrecies
		sys = varargin{1};
		A = sys.A;
		B = sys.B;
		C = sys.C;
		D = sys.D;
		delay = sys.delay;
		sampleTime = sys.sampleTime;

		% Create new feedback model
		switch regulatorNumber
			case 2 % Standard LQR
				%{
					 From this:
						 dx = Ax + Bu
						 y = Cx + Du
						 u = -uf + r = -Lx + r % Feedback control law
					 To this:
						 dx = (A-BL)x + Br
						 [y;uf]	= [C; L]x + [D;0]r
				%}

				% Create the A matrix
				A = (A-B*L);
				%B matrix is the same
				C = [C;L]; % This is for the uf = L*x control law
				D = [D; D*0]; % D need to have the same row length as C

				regsys = ss(delay, A, B, C, D);
				regsys.sampleTime = sampleTime;

			case 3 % LQR with integral action LQI
				% Create A matrix
				A = [(A-B*L) B*Li; (D*L-C) -D*Li];

				% Create B matrix
				ny = size(C, 1); % Number outputs
				nu = size(B, 2); % Number inputs
				B = [0*B; ones(ny, nu)]; % <- precompensator for reference = 0

				% Create C matrix
				C = [(C-D*L) D*Li];

				% Matrix D will be created by it self

				regsys = ss(delay, A, B, C, D);
				regsys.sampleTime = sampleTime;
			case 4 % LQR with integral action LQI + precompensator for reference
				% Create A matrix
				A = [(A-B*L) B*Li; (D*L-C) -D*Li];

				% Create B matrix
				ny = size(C, 1); % Number outputs
				nu = size(B, 2); % Number inputs
				B = [B; ones(ny, nu)];

				% Create C matrix
				C = [(C-D*L) D*Li];

				% Matrix D will be created by it self

				regsys = ss(delay, A, B, C, D);
				regsys.sampleTime = sampleTime;
		 end

	elseif(strcmp(type, 'TF' ))
		disp('Only state space models')
	else
		error('This is not TF or SS');
	end
end
