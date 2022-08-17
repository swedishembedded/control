% SPDX-License-Identifier: MIT
% Generates the minimal realization of a transfer function by cancle out poles
% against zeros wiht tolerance tol
% Gernerates the minimal Ho-Kalman-Kung realization of a state space model by cansle out
% non-controllable and non-observable states
% The arbitrary variables r and s controlls the dimension of Hankel Matrix
% Input: TF or SS, tol(optinal), r, s
% Example 1: G = minreal(G, tol)
% Example 2: sys = minreal(sys, s, r)
% Author: Daniel Mårtensson, 2017 Oktober

function [model] = minreal(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing model')
	end

	% Get tolerance
	if(length(varargin) < 2)
		tol = 1e-6;
	else
		tol = varargin{2};
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))

		% Get s and r
		if(length(varargin) > 2)
			s = varargin{2};
			r = varargin{3};
		else
			error('Missing r or s');
		end

		% Create discrete model - Important for minimal realization
		if(varargin{1}.sampleTime > 0) % Allready Discrete!
			sysd = varargin{1};
		else % Time continous
			sysd = c2d(varargin{1}, 0.1); % Sample time is 0.1 second
		end

		% Hankel matrix 0
		k = 0;
		H0 = obsv(sysd, r)*sysd.A^k*ctrb(sysd, s);
		[U,E,V] = svd(H0,'econ');

		% New rank n for finding En, Un, Vn
		n = rank(E);
		En = E(1:n, 1:n);
		Un = U(:, 1:n);
		Vn = V(:, 1:n);

		% Create identity matrecies
		p = size(sysd.C, 1); % number of outputs
		m = size(sysd.B, 2); % number of inputs
		Ey = [eye(p,p) zeros(p, (r-1)*p)]';
		Eu = [eye(m,m) zeros(m, (s-1)*m)]';

		% Hankel matrix 1
		k = 1;
		H1 = obsv(sysd, r)*sysd.A^k*ctrb(sysd, s);

		% The new minimal state space realization
		A = En^(-1/2)*Un'*H1*Vn*En^(-1/2);
		B = En^(1/2)*Vn'*Eu;
		C = Ey'*Un*En^(1/2);
		D = sysd.D;
		model = ss(varargin{1}.delay, A, B, C, D);
		if(varargin{1}.sampleTime > 0)
			% The model was discrete in the input
			model.sampleTime = varargin{1}.sampleTime;
		else
			% The model was continous in the input
			model.sampleTime = 0.1;
			model = d2c(model); % Turn it back!
		end
		% Done!
	elseif(strcmp(type, 'TF' ))
		% Get poles
		model = varargin{1};
		p = pole(model);
		% Get zeros
		[z, k] = zero(model);
		% Get delay
		delay = model.delay;
		% Get sample time
		sampleTime = model.sampleTime;

		% Check in which element they are equal
		[a, b, c] = intersect(round(z*1/tol)*tol, round(p*1/tol)*tol);
		% Remove zeros and poles if they are equal!
		z(b) = [];
		p(c) = [];

		% Check if the model has delay
		if(delay > 0)
			model = zpk(z, p, k, delay);
		else
			model = zpk(z, p, k);
		end

		% Insert sample time
		model.sampleTime = sampleTime;

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
		error('This is not TF or SS');
	end

end
