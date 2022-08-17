% SPDX-License-Identifier: MIT
% Get the covariance matrix of a state space model with the intensity of
% Gaussian white noise matrix
% Input: sys, W
% Example: [P] = covar(sys, W)
% Author: Daniel Mårtensson, Oktober 2017

function [P] = covar(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing model')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get sys
		sys = varargin{1};
		% Get sample time
		sampleTime = sys.sampleTime;
		% Get matrecies
		A = sys.A;
		B = sys.B;
		C = sys.C;
		D = sys.D;

		% Get W matrix
		if(length(varargin) >= 2)
			W = varargin{2};
		else
			erro('Missing W matrix')
		end

		% Check W matrix
		if(or(size(B,2) ~= size(W,1), size(W, 1) ~= size(W,2)))
			error('W matrix need to be square or B matrix need to have same columns as rows as W matrix')
		end

		if(~issymmetric(W))
			error('W-matrix need to be symmetric')
		end

		% Check if W matrix is semi-definite
		EigenValues = eig(W);
		for i = 1:length(EigenValues)
			if(EigenValues(i) < 0)
				error('W matrix need to have non negative eigen values')
			end
		end

		% Get lyapunov
		if(sampleTime > 0)
			opt = 'd';
		else
			opt = 't';
		end

		% Get lyapunov solution
		X = lyap(A, B*W*B', opt);

		% Get P matrix
		P = C*X*C' + D*W*D';
	elseif(strcmp(type, 'TF' ))
		error('Model must be a state space model')
	else
		error('This is not TF or SS');
	end

end
