% SPDX-License-Identifier: MIT
% Plot the nyquist diagram of a state space model or a transfer function
% between given frequencies w1 and w2
% Input: G, sys, w1, w2
% Example 1: [Margin_Sensitivity] = sensitivity(sys, w1, w2)
% Example 2: [Margin_Sensitivity] = sensitivity(G, w1, w2)
% Author: Daniel Mårtensson, 2017 Oktober

function [Ms] = sensitivity(varargin)
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
		% Call sensitivity
		Ms = sensitivity(G, w1, w2);
	elseif(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
		Ms =	zeros(size(G,1), size(G,2)); % The return matrix
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

				L = 10000;																 % Number of frequency elements - Need to be 10000 for the nyquist plot
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
				% Plot nyquist diagram
				% Done!
				% Plot nyquist diagram

				% Get Ms
				Ms = min(1+H);
				Mt = 1.3;
				x0 = -Mt^2/(Mt^2 + 1);
				y0 = 0;
				r = Mt/(Mt^2+1);

				% Get the Mt circle
				ang = 0:0.01:2*pi;
				xp = r*cos(ang);
				yp = r*sin(ang);

				figure('Name', sprintf(strcat('Transfer function: ', num2str(i), 'x', num2str(j))))
				plot([real(H)], [imag(H)],-1, 0 ,'+', linspace(-1, -abs(1-real(Ms))), linspace(0, imag(Ms)), x0 + xp, y0 + yp)
				title('Nyquist diagram')
				xlabel('Real axis')
				ylabel('Imaginary axis')
				grid on
				legend('real imag', 'instability point', 'max(S(jw))', 'max(T(jw)) = 1.3')

				%Ms(i,j) = abs(min(1/1+H));
				Ms(i,j) = abs(Ms);
			end
		end
	else
		error('Only transfer functions and state space models allowed')
	end
end

