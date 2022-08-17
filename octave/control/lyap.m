% SPDX-License-Identifier: MIT
% Compute solution X of the Lyapunov equation
% Input: A, Q, opt(optinal)
% Example 1: [X] = lyap(A, B*B'); % t = time contious as default
% Example 2: [X] = lyap(A, B*B', 'd'); % d = discrete
% Author: Daniel Mårtensson, Oktober 2017

function [X] = lyap(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing A, Q')
	end

	A = varargin{1};
	Q = varargin{2};

	if(length(varargin) <= 2)
		opt = 't';
	else
		opt = varargin{3};
	end

	if(strcmp(opt, 'd'))
		p = kron(conj(A), A);
		K = eye(size(p)) - p;
		X = K\Q(:);
		X = reshape(X, size(A));
	elseif(strcmp(opt, 't'))
		K = kron(eye(size(A)), A) + kron(conj(A), eye(size(A)));
		X = K\-Q(:);
		X = reshape(X, size(A));
	else
		disp(sprintf('Unknown option: %s', opt))
	end
end
