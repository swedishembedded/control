% SPDX-License-Identifier: MIT
% An IMC controller
% Input: G(System), K(Controller), Go(Forward Model)
% Example 1: [G] = imc(G, K, Go)
% Author: Daniel Mårtensson, Februari 2018


function [model] = imc(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Check if there is a second input
	if(isempty(varargin{2}))
		error ('Missing controller transfer function')
	end

	% Check if there is a third input
	if(isempty(varargin{3}))
		error ('Missing forward Modelr')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'TF' ))
		% Get system G
		G = varargin{1};
		% Get controller K
		K = varargin{2};
		% Get system Go
		Go = varargin{3};

		% They must have the same sample time
		if(and(G.sampleTime == K.sampleTime, G.sampleTime == Go.sampleTime))
			% Get num and den
			num1 = G.num;
			den1 = G.den;
			num2 = Go.num;
			den2 = Go.den;

			% Extend - pad - Equal length
			num1 = [zeros(1, length(num2) - length(num1)) num1];
			den1 = [zeros(1, length(den2) - length(den1)) den1];
			num2 = [zeros(1, length(num1) - length(num2)) num2];
			den2 = [zeros(1, length(den1) - length(den2)) den2];

			% Substract - Importat to have -conv(num1, den2) + conv(num2, den1)
			% because we are doing negative feedback - This is parallel method of two TF's
			num = conv(num1, den2) - conv(num2, den1);
			den = conv(den1, den2);

			% Create a transfer function of num and den
			GGo = tf(num, den);
			% Need to have the same sample time as G!
			GGo.sampleTime = G.sampleTime;
			% Do feedback witg GGo and K.
			F = feedback(K, GGo);
			% Do serial connection with F and G.
			model = series(F, G);
		else
			error('Not the same sample time')
		end
	else
		error('Only transfer functions');
	end

end
