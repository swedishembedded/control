% SPDX-License-Identifier: MIT
% Generates the gramian a state space model, where opt = 'c' or opt = 'o'
% Input: sys, opt
% Example 1: X = gram(sys, opt)
% Author: Daniel Mårtensson, Oktober 2017

function [X] = gram(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing model')
	end

	if(length(varargin) >= 2)
		opt = varargin{2};
	else
		error('Missing option: ´c´ or ´o´');
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get sys
		sys = varargin{1};
		% Get sample time
		sampleTime = varargin{1}.sampleTime;

		if(sampleTime > 0)
			% Discrete way
			A = sys.A;
			B = sys.B;
			C = sys.C;

			if(strcmp(opt, 'c'))
				X = lyap(A, B*B', 'd');
			elseif(strcmp(opt, 'o'))
				X = lyap(A', C'*C, 'd');
			else
				disp(sprintf('Unknown option: %s', opt));
			end
		else
			% Continous way
			A = sys.A;
			B = sys.B;
			C = sys.C;

			if(strcmp(opt, 'c'))
				X = lyap(A, B*B');
			elseif(strcmp(opt, 'o'))
				X = lyap(A', C'*C);
			else
				disp(sprintf('Unknown option: %s', opt));
			end
		end
	elseif(strcmp(type, 'TF' ))
		disp('Only state space model are allowed')
	else
		error('This is not TF or SS');
	end
end
