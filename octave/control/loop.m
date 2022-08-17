% SPDX-License-Identifier: MIT
% Create the Complementary sensitivity, sensitivity of an open loop transfer function L
% Input: L
% Example 1: [S, T] = loop(L)
% Author: Daniel Mårtensson, Oktober 2017
function [S, T] = loop(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing transfer function')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		error('Only loop transfer functions');
	elseif(strcmp(type, 'TF' ))
		% Get open loop transfer function
		L = varargin{1};
		% Create S
		N = tf(1,1); % Create a constant only for sensitivity function
		S = feedback(L, N);
		% Create T
		T = feedback(L, L);
	else
		error('This is not TF or SS');
	end
end
