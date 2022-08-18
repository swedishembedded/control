% SPDX-License-Identifier: MIT
% Author: Daniel Mårtensson, Oktober 2017
% Append diffrent models into one large model and get a state space model
% Input: G, sys
% Example 1: sys = append(G1, G2, sys1)
% Example 2: sys = append(sys1, sys2, G1, G2)

function [sys] = append(varargin)
	if(isempty(varargin{1}))
		error ('Missing model')
	end

	% Get type from the first model
	type = varargin{1}.type;

	% State space
	if(or(strcmp(type, 'SS' ), strcmp(type, 'TF' )))
		for i = 1:length(varargin)
			% check if we need to convert that TF to SS
			type = varargin{i}.type;
			if(strcmp(type, 'TF' ))
				% Yes!
				sys = tf2ss(varargin{i}, 'OCF');
			else
				% No!
				sys = varargin{i};
			end
			% Get the matrecies
			A = sys.A;
			B = sys.B;
			C = sys.C;
			D = sys.D;

			% The first one sets the default delay and sample time
			if(i == 1)
				firstdelay = sys.delay;
				firstsampleTime = sys.sampleTime;
				Apast = blkdiag([],A);
				Bpast = blkdiag([],B);
				Cpast = blkdiag([],C);
				Dpast = blkdiag([],D);
			else
				delay = sys.delay;
				sampleTime = sys.sampleTime;
				% The delay and sample time needs to be the same as the others
				if(and(firstdelay == delay, firstsampleTime == sampleTime))
					Apast = blkdiag(Apast,A);
					Bpast = blkdiag(Bpast,B);
					Cpast = blkdiag(Cpast,C);
					Dpast = blkdiag(Dpast,D);
				else
					error('Sample time or delay is not equal as the first model')
				end
			end
		end
		% Now we are done! Create the SS model now!
		sys = ss(firstdelay, Apast, Bpast, Cpast, Dpast);
		sys.sampleTime = firstsampleTime;
	else
		error('No state space model or transfer function')
	end
end
