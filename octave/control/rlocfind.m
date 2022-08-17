% SPDX-License-Identifier: MIT
% Author: Daniel Mårtensson, November 2017

function [K] = rlocfind (varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get pole location
	if(length(varargin) >= 2)
		p = varargin{2};
	else
		error('Missing pole location');
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% SS to TF
		G = ss2tf(varargin{1});
		[k, poles] = rlocfind(G, p);
	elseif(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
		K = zeros(size(G,1), size(G,2));

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

				N = length(b);
				h = sampleTime;
				% Notice that a and b has been switched, compared to bode.m
				if sampleTime > 0
					K(i,j) = abs((b*fliplr((exp(p*h)).^(0 : N-1)).')/(a*fliplr((exp(p*h)).^(0 : N-1)).')*exp(-delay*exp(p*h)));
				else
					K(i,j) = abs(b*fliplr((p).^(0 : N-1)).'/(a*fliplr(p.^(0 : N-1)).')*exp(-delay*p));
				end

			end
		end

	else
		error('Error model structure - Unknow');
	end
endfunction
