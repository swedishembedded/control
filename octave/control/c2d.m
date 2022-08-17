% SPDX-License-Identifier: MIT
% Transform continuous time model to discrete model
% Input: sys, G, sampeltime
% Example 1: Gd = c2d(G, sampleTime)
% Example 2: sysd = c2d(sys, sampleTime)
% Author: Daniel Mårtensson, September 2017

function [model] = c2d(varargin)
	if(isempty(varargin{1}))
		error ('Missing model')
	end

	if(isempty(varargin{2}))
		error ('Missing sampeltime')
	end

	% State space
	if(strcmp(varargin{1}.type, 'SS' ))
		% Get info
		A = varargin{1}.A;
		B = varargin{1}.B;
		C = varargin{1}.C;
		D = varargin{1}.D;
		delay = varargin{1}.delay;
		h = varargin{2};
		% Compute sizes
		a1 = size(A,2) + size(B,2) - size(A,1);
		b1 = size(A,2);
		a2 = size(A,2) + size(B,2) - size(B,1);
		b2 = size(B,2);
		% Compute square matrix
		M = [A B; zeros(a1, b1)	zeros(a2, b2)];
		M = expm(M*h);
		 % Find the discrete matrecies
		Ad = M(1:size(A,1), 1:size(A,2));
		Bd = M(1:size(B,1), (size(A,2) + 1):(size(A,2) + size(B,2)));
		model = ss(delay, Ad, Bd, C, D);
		% Don't forget to add sample time
		model.sampleTime = h;
	% Transfer function
	elseif(strcmp(varargin{1}.type, 'TF' ))
		% Transform it to a state space model.
		sys = tf2ss(varargin{1}, 'OCF');
		% Turn sys to discrete
		h = varargin{2};
		if(and(h > 0, sys.delay > 0))
			error('You cannot turn time continous transfer function with delay into discrete transfer function. Try state space instead: sys = tf2ss(G) -> sysd = c2d(sys)');
		end
		model = c2d(sys, h);
		% Turn it then into a tf
		model = ss2tf(model);
		model.sampleTime = h;
		% Replace the delaytime to discrete delay time
		model.tfdash = strrep(model.tfdash, 'e', 'z');
		model.tfdash = strrep(model.tfdash, 's', '');
		% Remove all s -> s
		model.tfnum = strrep(model.tfnum, 's', 'z');
		model.tfden = strrep(model.tfden, 's', 'z');
	else
		error('No state space model or transfer function')
	end
end
