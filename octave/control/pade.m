% SPDX-License-Identifier: MIT
% Do a pade approximation of a transfer function
% Input: G, n
% Example 1: G = pade(G, n)
% Author: Daniel Mårtensson, Februari 2018

function [model] = pade(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error('Missing model')
	end

	% Check if there is any input
	if(length(varargin) < 2)
		error('Please, insert the order of Pade Approximation: 1, 2, 3 or 4')
	end

	% Get the type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'TF' ))
		% If there is a MIMO TF
		G = varargin{1};
		PadeOrder = varargin{2};
		for i = 1:size(G,1)
			for j = 1:size(G,2)
				% Get numerator vector and denomerator vector
				a = G(i,j).num;
				b = G(i,j).den;
				% Get delay
				delay = G(i,j).delay;
				% Get sample time
				sampleTime = G(i,j).sampleTime;
				if(and(sampleTime > 0, delay > 0))
					error('You cannot turn time continous transfer function with delay into discrete transfer function. Try state space instead: sys = tf2ss(G) -> sysd = c2d(sys)');
				end

				switch PadeOrder
					case 1
							TFdelay = tf([-delay 2],[delay 2]);
					case 2
							TFdelay = tf([delay^2 -6*delay 12],[delay^2 6*delay 12]);
					case 3
							TFdelay = tf([-delay^3 12*delay^2 -60*delay 120],[delay^3 12*delay^2 60*delay 120]);
					case 4
							TFdelay = tf([delay^4 -20*delay^3 180*delay^2 -840*delay 1680],[delay^4 20*delay^3 180*delay^2 840*delay 1680]);
					otherwise
							error('Maximum Pade number is 4')
				end

				% Need to have the same sample time. Or else series.m won't work for us
				TFdelay.sampleTime = sampleTime;

				% Remove the delay because we don't want e^(-delay*s) notion
				G(i,j).delay = 0;
				model = series(G(i,j), TFdelay);
				% Add sample time
				model.sampleTime = sampleTime;
				model.delay = delay;
			end
		end
	else
		error('Only transfer functions');
	end
end
