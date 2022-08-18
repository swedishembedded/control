% SPDX-License-Identifier: MIT
% Generate a transfer function from vector of zero, vector of poles and vectior of gain
% Input: z, p , k, delay(optional)
% Example 1: G = zpk(z, p, k)
% Example 2: G = zpk(z, p, k, delay)
% Author: Daniel Mårtensson, 2017 September


function [G] = zpk(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get zeros
	if(length(varargin) < 1)
		error('Missing zeros')
	else
		z = varargin{1};
	end
	% Get poles
	if(length(varargin) < 2)
		error('Missing zeros')
	else
		p = varargin{2};
	end

	% Get gain
	if(length(varargin) < 3)
		error('Missing gain')
	else
		k = varargin{3};
	end

	% Check if values are cells
	if(~iscell(z))
		z = {z};
	end

	if(~iscell(p))
		p = {p};
	end

	% The gain k is never cell

	% Get numerator and denomerator
	num = cellfun (@(zer, gain) real (gain * poly (zer)), z, num2cell(k), 'uniformoutput', false);
	den = cellfun (@(pol) real (poly (pol)), p, 'uniformoutput', false);

	% Get delay
	if(length(varargin) >= 4)
		delay = varargin{4};
		G = tf(round(cell2mat(num)*1/1e-6)*1e-6, round(cell2mat(den)*1/1e-6)*1e-6, delay); % Tar bort alla 000000 på slutet
	else
		G = tf(cell2mat(num), cell2mat(den));
	end


end
