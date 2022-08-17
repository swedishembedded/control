% SPDX-License-Identifier: MIT
% Get the hankel singular values of a state space model
% Input: SS, p(optional)
% Example 1: hsv = hsvd(sys)
% Example 1: hsv = hsvd(sys, p) % p = 'plot'
% Author: Daniel Mårtensson, Oktober 2017

function [hsv] = hsvd(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing model')
	end

	if(length(varargin) >= 2)
		p = varargin{2};
	else
		p = 'n';
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get sys
		sys = varargin{1};
		% Get gramians
		P = gram(sys, 'c');
		Q = gram(sys, 'o');
		% Get hankel singular values
		hsv = sqrt(eig(P*Q));
		% Plot them too!
		if(strcmp(p,'plot'))
			bar(hsv)
			title('Hankel Singular Values (State Contributions)')
			xlabel('State')
			ylabel('State Energy')
			legend('Stable modes')
		end
	elseif(strcmp(type, 'TF' ))
		error('Model must be a state space model')
	else
		error('This is not TF or SS');
	end
end
