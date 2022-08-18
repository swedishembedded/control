% SPDX-License-Identifier: MIT
% Generates the low frequency gain of a state space model or a transfer function
% Input: sys, G
% Example 1: dc = dcgain(sys)
% Example 2: dc = dcgain(G)
% Author: Daniel Mårtensson 2017 September

function [dc] = dcgain(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get the type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get necessary info
		A = varargin{1}.A;
		B = varargin{1}.B;
		C = varargin{1}.C;
		D = varargin{1}.D;
		dc = C*inv(-A)*B + D;
	elseif(strcmp(type, 'TF' ))
		% Get necessary info
		for i = 1:size(varargin{1},1)
			for j = 1:size(varargin{1},2)
				% Get necessary info
				G = varargin{1}(i, j);
				% Get the static gain
				dc(i, j) = G.num(length(G.num))/G.den(length(G.den));
				% If divided by zero - Is not a number
				if isnan(dc(i, j))
					disp(sprintf('Divided my zero - dcgain (%i, %i) set to 0', i, j))
					dc(i, j) = 0;
				end
			end
		end
	else
		error('This is not TF or SS');
	end
end
