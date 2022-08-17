% SPDX-License-Identifier: MIT
% Do a serial connection of transfer functions or state space models
% Input: G, sys
% Example 1: G = series(G1, G2)
% Example 2: sys = series(sys1, sys2)
% Author: Daniel Mårtensson, 2017 Oktober

function [model] = series(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Check if there is a second input
	if(isempty(varargin{2}))
		error ('Missing second model')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get info
		sys1 = varargin{1};
		sys2 = varargin{2};
		if(sys1.sampleTime == sys2.sampleTime)
			% Get matrecies
			A1 = sys1.A;
			A2 = sys2.A;
			B1 = sys1.B;
			B2 = sys2.B;
			C1 = sys1.C;
			C2 = sys2.C;
			D1 = sys1.D;
			D2 = sys2.D;

			% Get big state space
			A = [A1 zeros(size(A1, 1), size(A2, 2)); B2*C1 A2];
			B = [B1; B2*D1];
			C = [D2*C1 C2];
			D = D2*D1;
			delay = sys1.delay + sys2.delay;
			model = ss(delay, A, B, C, D);
			model.sampleTime = sys1.sampleTime;
		else
			error('Need to have the same sampling time')
		end
	elseif(strcmp(type, 'TF' ))
		% Get TF
		G1 = varargin{1};
		G2 = varargin{2};
		if(G1.sampleTime == G2.sampleTime)
			% Get num and den
			num1 = G1.num;
			den1 = G1.den;
			num2 = G2.num;
			den2 = G2.den;
			% Get delay
			delay = G1.delay + G2.delay;
			num = conv(num1, num2);
			den = conv(den1, den2);
			if(delay > 0)
				model = tf(num, den, delay);
			else
				model = tf(num, den);
			end
			model.sampleTime = G1.sampleTime;

			% Discrete or not?
			if (model.sampleTime > 0)
				% Replace the delaytime to discrete delay time
				model.tfdash = strrep(model.tfdash, 'e', 'z');
				model.tfdash = strrep(model.tfdash, 's', '');
				% Remove all s -> s
				model.tfnum = strrep(model.tfnum, 's', 'z');
				model.tfden = strrep(model.tfden, 's', 'z');
			end
		else
			error('Need to have the same sampling time')
		end
	else
		error('This is not TF or SS');
	end
end
