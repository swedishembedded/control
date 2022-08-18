% SPDX-License-Identifier: MIT
% Plot poles and zeros of a transfer function or of a state space model
% Input: G, sys
% Example 1: [p, z] = pzmap(G)
% Example 2: [p, z] = pzmap(sys)
% Author: Daniel Mårtensson, 2017 September

function [p, z] = pzmap(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get poles
		p = pole(varargin{1})
		z = tzero(varargin{1})
		plot(real(p), imag(p), 'o', real(z), imag(z), 'x');
		grid on
		title('Pole-Zero Map')
		xlabel('Real axis');
		ylabel('Imaginary axis');

		% If we have no zeros - It can happen!
		if isempty(z)
			legend('poles');
		else
			legend('poles', 'zeros');
		end
	elseif(strcmp(type, 'TF' ))
		% Get G
		G = varargin{1};
		% Get poles and zeros
		z = roots(G.num);
		p = roots(G.den);
		plot(real(p), imag(p), 'o', real(z), imag(z), 'x');
		grid on
		title('Pole-Zero Map')
		xlabel('Real axis');
		ylabel('Imaginary axis');

		% If we have no zeros - It can happen!
		if isempty(z)
			legend('poles');
		else
			legend('poles', 'zeros');
		end
	else
		error('This is not TF or SS');
	end
end
