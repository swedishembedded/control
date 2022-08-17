% SPDX-License-Identifier: MIT
% Transform state space model to transfer function
% Input: sys
% Example 1: G = ss2tf(sys)
% Author: Daniel Mårtensson, 2017 September

function [G] = ss2tf(varargin)
	% Check if there is some input arguments or it's not a state space model
	if(or(isempty(varargin{1}), ~strcmp(varargin{1}.type, 'SS' )))
		error ('Missing state space model')
	end

	% Get the matrecies
	A = varargin{1}.A;
	B = varargin{1}.B;
	C = varargin{1}.C;
	D = varargin{1}.D;
	% Get the delay
	delay = varargin{1}.delay;
	% Get sampleTime
	sampleTime = varargin{1}.sampleTime;
	% Get the dc gain
	dc = dcgain(varargin{1});
	% Get the poles
	p = pole(varargin{1});

	% Create SISO state space models and convert them into a TF
	for i = 1:size(B,2)
		for j = 1:size(C,1)
			partialSys = ss(delay, A, B(:,i), C(j,:), D(j,i));
			% Get the z,p,k values
			[z, gain] = tzero(partialSys);
			% Get the numerators and denomerators
			partialG = zpk(z, p, gain);
			partialnum = partialG.num;
			partialden = partialG.den;
			% Get TF with delay
			if(delay > 0)
				G(j,i) = tf(partialnum, partialden, delay);
			else
				G(j,i) = tf(partialnum, partialden);
			end

			% Get sample time
			G(j,i).sampleTime = sampleTime;
			% If the model is discrete - do this!
			if(sampleTime > 0)
				% Replace the delaytime to discrete delay time
				G(j,i).tfdash = strrep(G(j,i).tfdash, 'e', 'z');
				G(j,i).tfdash = strrep(G(j,i).tfdash, 's', '');
				% Remove all s -> s
				G(j,i).tfnum = strrep(G(j,i).tfnum, 's', 'z');
				G(j,i).tfden = strrep(G(j,i).tfden, 's', 'z');
			end
			% Done!
		end
	end
end
