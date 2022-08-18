% SPDX-License-Identifier: MIT
% Computes the frequency response from a transfer function or state space model
% with the frequency vector w
% Input: sys, G, w
% Example 1: [H, wout] = freqresp(sys, w)
% Example 2: [H, wout] = freqresp(G, w)
% Author: Daniel Mårtensson, Oktober 2017

function [H, wout] = freqresp(varargin)
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
		% Call freqresp
		[H, wout] = freqresp(G, w);
	elseif(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
		% Create H
		L = length(w);	% Number of frequency elements
		H = zeros(size(G,1),size(G,2), L);
		for i = 1:size(G,1)
			for j = 1:size(G,2)
				% Get numerator vector and denomerator vector
				a = G(i,j).num;
				b = G(i,j).den;
				% Get delay
				delay = G(i,j).delay;
				% Get sample time
				sampleTime = G(i,j).sampleTime;

				% Numerator and denomerator need to be the same length
				if(length(a) > length(b))
					b = [zeros(1, size(a,2) - size(b,2)) b];
				elseif(length(a) < length(b))
					a = [zeros(1, size(b,2) - size(a,2)) a];
				end

				N = length(b); % Number of denomerators
				wout = w;
				% Evaluate transfer function
				h = sampleTime;
				if(sampleTime > 0) % Discrete model
					for k = 1 : L
						H(i,j,k) = (a*fliplr((exp(1i*wout(k)*h)).^(0 : N-1)).')/(b*fliplr((exp(1i*wout(k)*h)).^(0 : N-1)).')*exp(-delay*exp(1i*wout(k)*h));
					end
				else % Continous
					for k = 1 : L
						H(i,j,k) = (a*fliplr((1i*wout(k)).^(0 : N-1)).')/(b*fliplr((1i*wout(k)).^(0 : N-1)).')*exp(-delay*1i*wout(k));
					end
				end
				% Done!
			end
		end
	else
		error('Only transfer functions and state space models allowed')
	end
end
