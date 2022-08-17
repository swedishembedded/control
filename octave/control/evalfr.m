% SPDX-License-Identifier: MIT
% Computes the frequency response from a transfer function or state space model
% at single frequency w
% Input: G, sys, w
% Example 1: frsp = evalfr(sys, w)
% Example 2: frsp = evalfr(G, w)
% Author: Daniel Mårtensson, Oktober 2017

function [frsp] = evalfr(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error('Missing model')
	end

	% Check if there is any input
	if(length(varargin) < 2)
		error('Missing frequencies')
	end

	w = varargin{2};

	% Get the type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% SS to TF
		G = ss2tf(varargin{1});
		% Call evalfr
		[frsp] = evalfr(G, w);
	elseif(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
		% Create H
		%L = 1000;	% Number of frequency elements
		H = zeros(size(G,1),size(G,2), 1);
		for i = 1:size(G,1)
			for j = 1:size(G,2)
				% Get numerator vector and denomerator vector
				a = G(i,j).num;
				b = G(i,j).den;
				% Get delay
				delay = G(i,j).delay;

				% Numerator and denomerator need to be the same length
				if(length(a) > length(b))
					b = [zeros(1, size(a,2) - size(b,2)) b];
				elseif(length(a) < length(b))
					a = [zeros(1, size(b,2) - size(a,2)) a];
				end

				N = length(b); % Number of denomerators
				% Evaluate transfer function
				for k = 1 : 1
					H(i,j,k) = (a*fliplr((w(k)).^(0 : N-1)).')/(b*fliplr((w(k)).^(0 : N-1)).')*exp(-delay*w(k));
				end
				% Done!
			end
		end
		frsp = H;
	else
		error('Only transfer functions and state space models allowed')
	end
end
