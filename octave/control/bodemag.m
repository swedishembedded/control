% SPDX-License-Identifier: MIT
% Plot the bode diagram, without phase diagram, of a state space model or a
% transfer function between frequencies w1 and w2
% Input: sys, G, w1, w2
% Example 1:	bodemag(sys, w1, w2)
% Example 2:	bodemag(G, w1, w2)
% Author: Daniel Mårtensson, Oktober 2017

function [reval] = bodemag(varargin)
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
		% Call bode
		bode(G, w1, w2);
	elseif(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
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

				L = 1000;																	% Number of frequency elements
				N = length(b);														 % Number of denomerators
				w = logspace(log10(w1), log10(w2), L);		 % Angular frequencies
				% Evaluate transfer function
				H = zeros(1, L);
				h = sampleTime;
				if(sampleTime > 0) % Discrete model
					for k = 1 : L
						H(k) = (a*fliplr((exp(1i*w(k)*h)).^(0 : N-1)).')/(b*fliplr((exp(1i*w(k)*h)).^(0 : N-1)).')*exp(-delay*exp(1i*w(k)*h));
					end
				else
					for k = 1 : L
						H(k) = (a*fliplr((1i*w(k)).^(0 : N-1)).')/(b*fliplr((1i*w(k)).^(0 : N-1)).')*exp(-delay*1i*w(k));
					end
				end

				% Done!
				% Plot Bode diagram
				figure('Name', sprintf(strcat('Transfer function: ', num2str(i), 'x', num2str(j))))
				semilogx(w, 20*log10(abs(H)));
				ylabel('Magnitude [dB]');
				grid on
			end
		end
	else
		error('Only transfer functions allowed')
	end
end
