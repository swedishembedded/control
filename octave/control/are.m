% SPDX-License-Identifier: MIT
% Solve Algebraic Riccati Equation with the weighting matrices Q and R
% Input: sys, Q, R
% Example 1: [X] = are(sys, Q, R)
% Author: Daniel Mårtensson, Oktober 2017
% Updated: November 2017
% Updated: July 2020 - Added symmetric semi-positive definitive check for Q and R

function [X] = are(varargin)
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

	% Check if Q is symmetric semi-positive definite
	if(issymmetric(Q) == 0)
		error('Matrix Q is not symmetric')
	end
	if(eig(Q) < 0)
		error('Matrix Q is not semi-positive definite')
	end

	% Check if R is symmetric positive definite
	[~, P] = chol(R);
	if(P == 1)
		error('Matrix R is not symmetric positive definite')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get sys
		sys = varargin{1};

		% Get sample time
		sampleTime = sys.sampleTime;

		% Get matrices
		A = sys.A;
		B = sys.B;
		C = sys.C;

		% Get info
		ny = size(C, 1); % Number outputs
		nu = size(B, 2); % Number inputs
		nx = size(A, 1); % Number states

		% Check if Q have the right dimension
		if(nx ~= size(Q, 1))
			str = sprintf('Q need to have the dimension %ix%i', nx, nx);
			error(str);
		end

		% Check if R have the right dimension
		if(nu ~= size(R, 1))
			str = sprintf('R need to have the dimension %ix%i', nu, nu);
			error(str);
		end

		% Get initial conditions
		x0 = ones(size(A))(:); % Vector

		if(sampleTime > 0)
			% Create discrete algebraic riccati equation
			% Simulate
			[T, X] = ode45(@(t,X)dare(t, X, A, B, Q, R), [0 100], x0);
		else
			% Create continuos algebraic riccati equation
			% Simulate
			[T, X] = ode45(@(t,X)care(t, X, A, B, Q, R), [0 100], x0);
		end

		% Get the last value of X and turn it into a matrix_type
		X = reshape(X(size(X, 1), :), size(A)); % Here is the solution!
	elseif(strcmp(type, 'TF' ))
		error('Transfer function model not supported. Convert to state space.')
	else
		error('This is not TF or SS');
	end
end

function [value, isterminal, direction] = care(t, X, A, B, Q, R)
	X = reshape(X, size(A)); % Vector -> Matrix
	value = A'*X + X*A - X*B*inv(R)*B'*X + Q; % Value is the derivative of X
	value = value(:); % Matrix -> Vector

	isterminal = 1;
	direction = 0;
end

function [value, isterminal, direction] = dare(t, X, A, B, Q, R)
	X = reshape(X, size(A)); % Vector -> Matrix
	value = A'*X*A - X - A'*X*B*inv(R + B'*X*B)*B'*X*A + Q; % Value is the derivative of X
	value = value(:); % Matrix -> Vector

	isterminal = 1;
	direction = 0;
end
