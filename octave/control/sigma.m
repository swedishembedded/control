% SPDX-License-Identifier: MIT
% Plot sigma diagram from a transfer function or state space model
% between given frequencies w1 and w2
% Input: G, sys, w1, w2
% Example 1: sigma(sys, w1, w2)
% Example 2: sigma(G, w1, w2)
% Author: Daniel Mårtensson, 2017 Oktober

function [retval] = sigma(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error('Missing model')
	end

	% Check if there is any input
	if(length(varargin) < 3)
		w1 = 0.01;
		w2 = 100;
	else
		w1 = varargin{2};
		w2 = varargin{3};
	end

	% Get the type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% SS to TF
		G = ss2tf(varargin{1});
		% Call sigma
		sigma(G, w1, w2);
	elseif(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
		% Create H
		L = 1000;	% Number of frequency elements
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
				w = logspace(log10(w1), log10(w2), L);		 % Angular frequencies
				% Evaluate transfer function
				% norm(A,2) = max(SVD(A))
				h = sampleTime;
				if(sampleTime > 0) % Discrete model
					for k = 1 : L
						H(i,j,k) = norm((a*fliplr((exp(1i*w(k)*h)).^(0 : N-1)).')/(b*fliplr((exp(1i*w(k)*h)).^(0 : N-1)).')*exp(-delay*exp(1i*w(k)*h)), 2);
					end
				else
					for k = 1 : L
						H(i,j,k) = norm((a*fliplr((1i*w(k)).^(0 : N-1)).')/(b*fliplr((1i*w(k)).^(0 : N-1)).')*exp(-delay*1i*w(k)), 2);
					end
				end

				% Done!
			end
		end
		% Plot now!
		legendStr = ['']; % Need to have '', or else legendStr is numeric
		for i = 1:size(H,1)
			for j = 1:size(H,2)
				y = H(i,j,:);
				semilogx(w, 20*log10(abs(y(:))), 'color', rand(1,3));
				grid on
				ylabel('Magnitude [dB]');
				xlabel('Frequency [rad/s]');
				title('Singular Values');
				legendStr = [legendStr strcat('TF:', num2str(i), 'x', num2str(j)), '#']; % # is there beacuse we need to split the string later
				hold on
			end
		end
		% Remove last #
		legendStr = strsplit(legendStr(1,1:end-1), '#');
		% Now plot the legendStr
		legend(legendStr);
		% Done!
	else
		error('Only transfer functions and state space models allowed')
	end
end
