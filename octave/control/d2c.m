% SPDX-License-Identifier: MIT
% Transform discrete transfer function or state space model to continuous time
% transfer function or state space model 
% Input: Gd, sysd
% Example 1: G = d2c(Gd)
% Example 2: sys = d2c(sysd)
% Author: Daniel Mårtensson, September 2017

function [model] = d2c(varargin)
	if(isempty(varargin{1}))
		error ('Missing model')
	end
	% State space
	if(strcmp(varargin{1}.type, 'SS' ))
		% Get info
		A = varargin{1}.A;
		B = varargin{1}.B;
		C = varargin{1}.C;
		D = varargin{1}.D;
		delay = varargin{1}.delay;
		sampleTime = varargin{1}.sampleTime;
		% Compute sizes
		a1 = size(A,2) + size(B,2) - size(A,1);
		b1 = size(A,2);
		a2 = size(A,2) + size(B,2) - size(B,1);
		b2 = size(B,2);
		% Compute square matrix
		M = [A B; zeros(a1, b1)	eye(a2, b2)];
		M = round(logm(M)*1/sampleTime*1/1e-6)*1e-6; % Very important to remove small numbers!!
		M(abs(M) < 1e-6) = 0; % Very importat to turn -0 to 0.
		A = M(1:size(A,1), 1:size(A,2));
		B = M(1:size(B,1), (size(A,2) + 1):(size(A,2) + size(B,2)));
		% Return model
		model = ss(delay, A, B, C, D);
		% Don't forget to add sample time to zero again
		model.sampleTime = 0;
	elseif(strcmp(varargin{1}.type, 'TF' ))
		% Turn TF to SS
		sysd = tf2ss(varargin{1}, 'OCF');
		% Call d2c2
		G = d2c(sysd);
		% SS to SS
		model = ss2tf(G);
	else
		error('No state space model or transfer function')
	end
end
