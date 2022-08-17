% SPDX-License-Identifier: MIT
% An Otto Smith predictor
% Input: G(System with delay), K(Controller), n(Pade Approximation number)
% Example 1: [G] = smithpredict(G, K, n)
% Author: Daniel Mårtensson, Februari 2018
% Important! Discrete system may be very sensitive with Otto Smith predictor!

function [model] = smithpredict(varargin)
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
		error ('Missing pade approximation number')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'TF' ))
		% Get system G
		Gsystem = varargin{1};

		% Check if the system has delay
		if Gsystem.delay <= 0
			error('System missing delay.');
		end

		% Get pade approximation number
		n = varargin{3};

		% Create the system with delay
		Gdelaysystem = pade(Gsystem, n);

		% Get the controller transfer function
		Gcontroller = varargin{2};

		% Check if the Gcontroller has a delay too!
		if Gcontroller.delay > 0
			error('The controller has delay.');
		end

		% They must have the same sample time
		if(Gsystem.sampleTime == Gcontroller.sampleTime)
			% Get num and den
			num1 = Gdelaysystem.num;
			den1 = Gdelaysystem.den;
			num2 = Gsystem.num;
			den2 = Gsystem.den;

			% Extend - pad - Equal length
			num1 = [zeros(1, length(num2) - length(num1)) num1];
			den1 = [zeros(1, length(den2) - length(den1)) den1];
			num2 = [zeros(1, length(num1) - length(num2)) num2];
			den2 = [zeros(1, length(den1) - length(den2)) den2];

			% Substract - Importat to have -conv(num1, den2) + conv(num2, den1)
			% because we are doing negative feedback
			num = -conv(num1, den2) + conv(num2, den1);
			den = conv(den1, den2);

			% Create a transfer function of num and den
			Gdelaysubstractsystem = tf(num, den);
			% Need to have the same sample time as Gdelaysystem!
			Gdelaysubstractsystem.sampleTime = Gdelaysystem.sampleTime;
			% Create the predicive controller now
			predictController = feedback(Gcontroller, Gdelaysubstractsystem);
			% Connect predictController with Gdelay
			Looptransferfunction = series(Gdelaysystem, predictController);

			% This is only for scalar 1 in the feedback
			if Gsystem.sampleTime > 0
				M = tf(1,1); % Just a scalar 1 of TF
				M = c2d(M, Gsystem.sampleTime);
			else
				M = tf(1,1); % Just a scalar 1 of TF
			end

			% Our smitth predictor system!
			model = feedback(Looptransferfunction, M);
		else
			error('Not the same sample time')
		end
	else
		error('Only transfer functions');
	end

end
