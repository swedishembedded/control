% SPDX-License-Identifier: MIT
% Author: Daniel Mårtensson, Oktober 2017
% Print out all margins of a state space model or a
% transfer function between frequencies w1 and w2
% Input: sys, G, w1, w2
% Example 1: S = allmargin(sys, w1, w2)
% Example 2: S = allmargin(G, w1, w2)

function [S] = allmargin(varargin)
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
		% Call allmargin
		allmargin(G, w1, w2);
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

				L = 10000;																 % Number of frequency elements
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

				% Get wc, phim
				wc = inf;
				phim = inf;
				flag = false;
				for k = 1:length(H)
					% The dB need to be over 0 for flag = true
					if ((20*log10(abs(H(k))) > 0))
						flag = true;
					end
					% When dB is under 0 and flag = true
					if (and(20*log10(abs(H(k))) <= 0, flag == true ))
						wc = w(k);
						phim = 180 + angle(H(k)) * 180/pi;
						break;
					end
				end

				% Get wpi, Am
				wpi = inf;
				Am = inf;
				for k = 1:length(H)
					if (angle(H(k)) * 180/pi <= -180)
						wpi = w(k);
						Am = 20*log10(abs(H(k)));
						break;
					end
				end

				% Compute the delay margin
				Dm = pi*phim/(180*wc);

				% Check if it's stable
				p = pole(G(i,j));
				stable = 1;
				for k = 1:length(p)
					if(real(p(k)) > 0)
						stable = 0; % Not stable! We got one positive pole!
						break;
					end
				end

				% Am
				Am = abs(Am);

				% Print it out
				% DMFrequency(Phidm) is the same as wc
				S(i,j) = {sprintf('Transfer function: %ix%i\nGainMargin(Am): %i\nGMFrequency(wpi): %i\nPhaseMargin(Phim): %i\nPMFrequency(Wc): %i\nDelayMargin(Dm): %i\nDMFrequency(Phidm): %i\nStable: %i\n', i, j, Am, wpi, phim, wc, Dm, wc, stable)};


			end
		end
		% Do cell2mat
		S = cell2mat(S);
	else
		error('Only transfer functions and state space models allowed.')
	end
end
