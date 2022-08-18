% SPDX-License-Identifier: MIT
% Generates zeros from transfer functions or state space models
% Input: G, sys
% Example 1: [Frequency, Damping, Poles, TimeConstant]	= damp(G)
% Example 2: [Frequency, Damping, Poles, TimeConstant]	= damp(sys)
% Author: Daniel Mårtensson, September 2017

function [Frequency, Damping, Poles] = damp(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end
	% Get model type
	type = varargin{1}.type;
	% Get sampleTime
	sampleTime = varargin{1}.sampleTime;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Check if it's discrete
		if(sampleTime == 0)
			Poles = pole(varargin{1})
			Frequency = abs(Poles)
			Damping = -cos(angle(Poles))
			TimeConstant = (1./(Frequency.*Damping))
		else
			% Discrete
			Poles = pole(varargin{1})
			Frequency = abs(log(Poles)/sampleTime)
			Damping = -cos(angle(log(Poles)))
			TimeConstant = (1./(Frequency.*Damping))
		end
	elseif(strcmp(type, 'TF' ))
		% Check if it's discrete
		if(sampleTime == 0)
			Poles = pole(varargin{1})
			Frequency = abs(Poles)
			Damping = -cos(angle(Poles))
			TimeConstant = (1./(Frequency.*Damping))
		else
			% Discrete
			Poles = pole(varargin{1})
			Frequency = abs(log(Poles)/sampleTime)
			Damping = -cos(angle(log(Poles)))
			TimeConstant = (1./(Frequency.*Damping))
		end
	else
		error('This is not TF or SS');
	end
end
