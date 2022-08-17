% SPDX-License-Identifier: MIT
% Computes the new transfer function or state space model of serial connection
% Input: G, sys, sign(optinal)
% Example 1: G = feedback(G1, G2) % Negative feedback is used as default
% Example 2: sys = feedback(sys1, sys2) % Negative feedback is used as default
% Example 3: sys = feedback(sys1, sys2, sign) % sign = '+' Positive feedback
% Author: Daniel Mårtensson, Oktober 2017

function [model] = feedback(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Check if there is a second input
	if(isempty(varargin{2}))
		error ('Missing second model')
	end

	% Check feedback sign
	if(length(varargin) >= 3)
		f = varargin{3};
		if(or(strcmp(f,'+'),strcmp(f,'-')))
			f = varargin{3};
		else
			error('Unknow feedback sign');
		end
	else
		f = '-';
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

			% Get feedback state space
			if(strcmp(f, '-'))
				A = [(A1 - B1*D2*C1) -B1*C2 ; B2*C1 (A2 - B2*D1*C2)];
				B = [B1; B2*D1];
				C = [C1 -D1*C2];
				D = D1;
			else % f = '+'
				A = [(A1 + B1*D2*C1) B1*C2 ; B2*C1 (A2 + B2*D1*C2)];
				B = [B1; B2*D1];
				C = [C1 D1*C2];
				D = D1;
			end
			if(sys1.delay ~= sys2.delay)
				error('It need to be the same delay for both of the systems')
			end
			delay = sys1.delay;
			model = ss(delay, A, B, C, D);
			model.sampleTime = sys1.sampleTime;
		else
			error('Need to have the same sampling time')
		end
	elseif(strcmp(type, 'TF' ))
		% Get info
		G1 = varargin{1};
		G2 = varargin{2};
		if(G1.sampleTime == G2.sampleTime)
			% Get num and den for the model down
			G_down = series(G1, G2);
			num_down = G_down.num;
			den_down = G_down.den;
			% Get num and den for model above
			G_up = G1;
			num_up = G_up.num;
			den_up = G_up.den;
			% Get delay
			delay = G1.delay;

			% Need to have same length for the down model
			num_down = [zeros(1, length(den_down) - length(num_down)) num_down];
			den_down = [zeros(1, length(num_down) - length(den_down)) den_down];

			% -1 feedback or +1 feedback
			% 1 - a/b = (b-a)/b
			% 1 + a/b = (b+a)/b
			if(strcmp(f, '-'))
				num_down = den_down + num_down;
			else % f = '+'
				num_down = den_down - num_down;
			end

			% (a/b)/(c/d) = a*d/(b*c)
			num = conv(num_up, den_down);
			den = conv(den_up, num_down);

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
