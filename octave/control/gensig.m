% SPDX-License-Identifier: MIT
% Generates a signal of sin, square or pulse
% Input: type, amp, Tf, Ts
% Example 1: [u, t] = gensig(type, amp, Tf, Ts)
% Author: Daniel Mårtensson, Oktober 2017

function [u, t] = gensig(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error('Missing input')
	end
	% Get type
	if(length(varargin) > 0)
		type = varargin{1};
	else
		error('Missing type')
	end
	% Get the amplitude
	if(length(varargin) > 1)
		amp = varargin{2};
	else
		amp = 1;
	end
	% Get the Tf
	if(length(varargin) > 2)
		Tf = varargin{3};
	else
		Tf = 1;
	end
	% Get time
	if(length(varargin) > 3)
		Ts = varargin{4};
	else
		Ts = 10;
	end

	switch type
		case 'sin'
			t = linspace(0,Ts, 1000);
			u = amp*sin(Tf*t + Ts);
		case 'square'
			t = linspace(0,Ts, 1000);
			u = [];
			n = 1;
			if(Tf == 1)
				Tf = 2;
			end
			for i = 1:length(t)
				if(i >= length(t)/(Tf)*n)
					u(i) = amp;
					if(i >= length(t)/(Tf)*(n + 1))
						n = n + 2;
					end
				else
					u(i) = 0;
				end
			end
		case 'pulse'
			t = linspace(0,Ts, 1000);
			u = [];
			n = 1;
			if(Tf == 1)
				Tf = 2;
			end
			for i = 1:length(t)
				if(i >= length(t)/(Tf)*n)
					u(i) = amp;
					n = n + 1;
				else
					u(i) = 0;
				end
			end
			u(end) = 0; % Remove the last peak
	end
end
