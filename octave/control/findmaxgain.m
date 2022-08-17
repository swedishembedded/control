% SPDX-License-Identifier: MIT
% Find the maximum gain limit when the process becomes unstable.
% Input: sys, G, maxgain
% Example 1: [K] = findmaxgain(sys, maxgain)
% Example 2: [K] = findmaxgain(G, maxgain)
% Example 3: [K] = findmaxgain(G)
% Example 4: [K] = findmaxgain(sys)
% Author: Daniel Mårtensson, 2018 Februari

function [K] = findmaxgain(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error('Missing model')
	end

	 % Get the type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% SS to TF
		G = ss2tf(varargin{1});
		if(length(varargin) >= 2)
			maxgain = varargin{2};
		else
			%disp('Default gain is 1000');
			maxgain = 1000;
		end
		% Call findmaxgain
		findmaxgain(G, maxgain);
	elseif(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
		for i = 1:size(G,1)
			for j = 1:size(G,2)
				% Get sample time
				sampleTime = G(i,j).sampleTime;
				% Get num
				num = G(i,j).num;
				% Get den
				den = G(i, j).den;
				% Maximum Gain K
				if(length(varargin) >= 2)
					maxgain = varargin{2};
				else
					%disp('Default gain is 1000');
					maxgain = 1000;
				end

				% P-controller vector
				P = linspace(0, maxgain, 10000);

				% Turn them into equal length
				if length(den) < length(num)
					den = [zeros(1, length(num) - length(den)) den];
				elseif length(den) > length(num)
					num = [zeros(1, length(den) - length(num)) num];
				end

				% Do numerical calculation
				for k = 1:length(P)
					% Multiply P with num and add with den
					polynomal = P(k)*num + den; % Always begin with P(k) = 0.	G(s) = K*num + den is the feedback G(s) = K*G/(1 + K*G)
					% Find the roots
					PolyRoots = roots(polynomal);
					% Check if the system is discrete or not
					if sampleTime > 0
						% Check if PolyRoots are stable - Discrete way
						Temp = and(abs(PolyRoots) <= 1, abs(PolyRoots) >= -1);
						Stable = prod(Temp); % If someone is 0, then Stable will be 0
					else
						% Check if PolyRoots are stable - Discrete way
						Temp = real(PolyRoots) <= 0;
						Stable = prod(Temp); % If someone is 0, then Stable will be 0
					end

					% Check if Stable is 1 or 0
					if Stable == 0
						K(i, j) = P(k); % Save this K because this is the limit
						break; % Break the for-loop for the numerical calculation
					end
				end
			end
		end
	else
		error('Only transfer functions and state space models allowed')
	end
end
