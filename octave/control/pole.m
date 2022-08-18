% SPDX-License-Identifier: MIT
% Generates poles from transfer functions or state space models
% Input: TF or SS
% Example 1: p = pole(G)
% Example 2: p = pole(sys)
% Author: Daniel Mårtensson 2017 September

function [p] = pole(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get poles
		A = varargin{1}.A;
		p = eig(A); % Eigenvalues
	elseif(strcmp(type, 'TF' ))
		% Get poles
		G = varargin{1};
		p = roots(G.den);
	else
		error('This is not TF or SS');
	end
end
