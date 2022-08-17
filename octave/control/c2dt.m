% SPDX-License-Identifier: MIT
% Transform a state space model or transfer function to discrete state space model with delay
% Input: G, sys, sampleTime
% Output: sysd(discrete state space model with delay), l(step prediction)
% Example 1: [sysd, l] = c2dt(G, sampleTime)
% Example 2: [sysd, l] = c2dt(sys, sampleTime)
% Author: Daniel Mårtensson, Oktober 2017
% Updated: 2018-03-04

function [sysd, l] = c2dt(varargin)
	if(isempty(varargin{1}))
		error ('Missing model')
	end

	% Check sample time
	if(length(varargin) < 2)
		error ('Missing sample time')
	end

	% Get delay
	if(varargin{1}.delay <= 0)
		error('This model has no delay')
	end

	% Get type
	type = varargin{1}.type;

	% State space
	if(strcmp(type, 'SS' ))
		% Check if allready discrete
		if(varargin{1}.sampleTime > 0)
			% The model is allready discrete! Turn it back!
			sys = d2c(varargin{1}, varargin{1}.sampleTime);
			A = sys.A;
			B = sys.B;
			C = sys.C;
			D = sys.D;
			sampleTime = sys.sampleTime; % Over write the sampleTime
			delay = sys.delay; % Get delay
		else
			% No! The model is not discrete!
			sys = varargin{1};
			A = sys.A;
			B = sys.B;
			C = sys.C;
			D = sys.D;
			sampleTime = varargin{2}; % Get the sample time
			delay = sys.delay;
		end

		% Original Source: Digital Control Of Dynamic Systems - G.F Franklin
		% Modified by me!
		% Get sizes of input and output matrix
		[bx,by] = size(B);
		[cx,cy] = size(C);
		l = ceil(delay/sampleTime); % Check of delay is less that sampleTime - Round up
		m = l*sampleTime - delay;

		% Create discrete matrecies s1, s2 s3 etc
		s1 = expm([A*m, B*m; zeros(by,bx+by)]);
		s2 = expm([A*(sampleTime-m), B*(sampleTime-m); zeros(by,bx+by)]);
		s3 = eye((l-1)*by);
		s4 = zeros(cy,(l-2)*by);
		s5 = zeros((l-1)*by,bx+by);
		s6 = zeros(by,bx+l*by);

		P =	s1(1:bx,	1:bx)*s2(1:bx, 1:bx);
		H1 = s1(1:bx, 1:bx)*s2(1:bx, bx+1:bx+by);
		H2 = s1(1:bx, bx+1:bx+by);
		if(l == 1) % If the delay is less that sampleTime
			A = [P, H1; s6];
			B = [H2; eye(by)];
			C = [C, zeros(cx, by)];
			D = zeros(cx, by);
		else	% If the delay is more that sampleTime
			A = [P, H1, H2, s4; s5, s3; s6];
			B = [zeros(bx+(l-1)*by, by); eye(by)];
			C = [C,zeros(cx, l*by)];
			D = zeros(cx, by);
		end
		% Get the discrete state space model now
		sysd = ss(delay, A, B, C, D);
		sysd.sampleTime = sampleTime; % Get the sample time
	elseif(strcmp(type, 'TF' ))
		% Check if allready discrete
		if(varargin{1}.sampleTime > 0)
			% Yes! The model is discrete! Turn it back!
			G = d2c(varargin{1}, varargin{1}.sampleTime);
			% TF to SS
			sys = tf2ss(G, 'OCF');
			% Get the sample time
			sampleTime = varargin{1}.sampleTime;
			% Call c2dt
			[sysd, l] = c2dt(sys, sampleTime);
		else
			% TF to SS
			G = varargin{1};
			sys = tf2ss(G, 'OCF');
			% Get the sample time
			sampleTime = varargin{2};
			% Call c2dt
			[sysd, l] = c2dt(sys, sampleTime);
		end
	else
		error('No state space model or transfer function')
	end
end
