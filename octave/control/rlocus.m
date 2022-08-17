% SPDX-License-Identifier: MIT
% Plot root locus plot from a transfer function or state space model at given
% vector of gains gainvector
% Input: sys, G, gainvector
% Example 1: rlocus(sys, gainvector)
% Example 2: rlocus(G, gainvector)
% Author: Daniel Mårtensson, 2017 Oktober

function [retval] = rlocus(varargin)
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
			Kvector = varargin{2};
		else
			error('Missing K gain vector');
		end
		% Call rlocus
		rlocus(G, Kvector);
	elseif(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
		for i = 1:size(G,1)
			for j = 1:size(G,2)
				% Get sample time
				sampleTime = G(i,j).sampleTime;
				% Gain K vector
				if(length(varargin) >= 2)
					Kvector = varargin{2};
				else
					error('Missing K gain vector');
				end

				% Size of the pole and zeros
				[npole,mpole] = size(pole(G));
				[nzero,mzero] = size(zero(G));
				% Create empty vectors for storeing the zeros and poles
				pvector = [];%zeros(npole, length(Kvector));
				zvector = [];%zeros(nzero, length(Kvector));
				for k = 1:length(Kvector)
					if(sampleTime > 0) % Discret
						GK = tf(Kvector(k),1);
						GKd = c2d(GK, sampleTime); % Sampleing time need to be the same
						Gfeedback = feedback(G(i,j), GKd);
					else
						GK = tf(Kvector(k),1);
						Gfeedback = feedback(G(i,j), GK);
					end
					pvector(:,k) = pole(Gfeedback);
					zvector(:,k) = zero(Gfeedback);
				end
				% Done! Plot it now!

				figure('Name', sprintf(strcat('Transfer function: ', num2str(i), 'x', num2str(j))))
				plot(real(pvector), imag(pvector), '.r', real(zvector), imag(zvector), 'bo');
				grid on

				% Scale plot
				xlim([-10 10])
				ylim([-10 10])

				% Title
				title('Root Locus');
				xlabel('Real Axsis (Seconds^-1)');
				ylabel('Imaginary Axis (Seconds^-1)');

			end
		end
	else
		error('Only transfer functions and state space models allowed')
	end
end
