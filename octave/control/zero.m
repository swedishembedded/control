% SPDX-License-Identifier: MIT
% Generates zeros from SISO transfer function or SISO state space model
% Input: G, sys
% Example 1: [z, gain] = zero(G)
% Example 2: [z, gain] = zero(sys)
% Author: Daniel Mårtensson, 2017 September

function [z, gain] = zero(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		A = varargin{1}.A;
		sizeOrginalA = size(A, 2);
		B = varargin{1}.B;
		C = varargin{1}.C;
		D = varargin{1}.D;
		% Check if it's SISO system
		if(or(size(B, 2) > 1, size(C, 1) > 1))
			error('This is only for SISO system, try to use tzero insted')
		end
		z = eig([A B; C D], [eye(size(A,1)) B*0; C*0 D*0]); % Get zeros
		z = z(isfinite(z)); % Remove the inf and zero values from z
		% check the lengt of z so we can remove some 0 0 0 from z
		if(length(z) - sizeOrginalA > 0)
			% Ok! We have more zeros that orginal size of A
			% Remove all 0 0 0
			z = z(z ~= 0);
			% Then add new one!
			% if sizeOrginalA - length(z) <= 0, then it will be no change
			z = [z zeros(1, sizeOrginalA - length(z))]; % add 0 if needed
		end
		p = pole(varargin{1});
		dc = dcgain(varargin{1});
		gain = dc*prod(-p)/prod(-z);
	elseif(strcmp(type, 'TF' ))
		% Get z, p, k
		G = varargin{1};
		if(or(size(G,1) > 1, size(G,2) > 1))
			error('This is only for SISO system, try to use tzero insted')
		end
		z = roots(G.num);
		p = roots(G.den);
		dc = dcgain(G);
		gain = dc*prod(-p)/prod(-z);
	else
		error('This is not TF or SS');
	end
end
