% SPDX-License-Identifier: MIT
% Computes the reference gain Kr and give back a state space model with
% including the reference gain
% Input: sys
% Example 1: [sys_kr, Kr] = referencegain(sys)
% Author: Daniel Mårtensson, September 2018

function [sys_kr, Kr] = referencegain(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end
	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get A, B, C, D matrecies
		sys = varargin{1};
		A = sys.A;
		B = sys.B;
		C = sys.C;
		D = sys.D;
		delay = sys.delay;
		sampleTime = sys.sampleTime;

		if sampleTime > 0
			Kr = 1./(C*inv(eye(size(A)) - A)*B);
		else
			Kr = 1./(C*inv(-A)*B);
		end
		% Now create B matrix with precompensator factor - For better tracking
		B = B*Kr;

		sys_kr = ss(delay, A, B, C, D);
		sys_kr.sampleTime = sampleTime;

	elseif(strcmp(type, 'TF' ))
		disp('Only state space models')
	else
		error('This is not TF or SS');
	end
end
