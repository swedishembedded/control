% SPDX-License-Identifier: MIT
% Generates a state space model from matrix A, B, C, D
% Input: delay, A, B, C(optional), D(optional)
% Example 1: sys = ss(delay, A, B)				 % C set to diagnoal matrix
% Example 2: sys = ss(delay, A, B, C)			% D set to zero matrix
% Example 3: sys = ss(delay, A, B, C, D)
% Author: Daniel Mårtensson, 2017 September

function [sys] = ss(varargin)

	if(length(varargin) < 1)
		error('Missing delaytime, A-matrix and B-matrix')
	end

	if(length(varargin) < 2)
		error('Missing A-matrix and B-matrix')
	else
		sys.A = varargin{2};
		% Check if sys.A is square matrix
		if(size(sys.A,1) ~= size(sys.A,2))
			error('A is not a square');
		end
	end

	if(length(varargin) < 3)
		error('Missing B-matrix')
	else
		sys.B = varargin{3};
		% Check if sys.A and sys.B have the same lenght of rows
		if(size(sys.A, 1) ~= size(sys.B, 1))
			error('A and B have not the same row length');
		end
	end

	if(length(varargin) < 4)
		% sprintf('C matrix assumed to be a diagonal %ix%i matrix', size(varargin{2}, 1),size(varargin{2}, 2))
		sys.C = eye(size(varargin{2}, 1));
	else
		sys.C = varargin{4};
		% Check if sys.A and sys.C have the same lenght of columns
		if(size(sys.A, 2) ~= size(sys.C, 2))
			error('A and C have not the same column length');
		end
	end

	if(length(varargin) < 5)
		% sprintf('D matrix assumed to be a zero %ix%i matrix', size(sys.C, 1), size(sys.B, 2))
		sys.D = zeros(size(sys.C, 1), size(sys.B, 2));
	else
		sys.D = varargin{5};
		% Check if sys.C and sys.D have the same lenght of rows
		if(size(sys.C, 1) ~= size(sys.D, 1))
			error('C and D have not the same row length');
		end
		% and check if sys.B and sys.D have the same lenght of columns
		if(size(sys.B, 2) ~= size(sys.D, 2))
			error('D and B have not the same columns length');
		end
	end

	% Delay time
	sys.delay = varargin{1};
	sys.type = 'SS';
	%Sampel time
	sys.sampleTime = 0;
end
