% SPDX-License-Identifier: MIT
% Generates the MIMO zeros from a transfer function or state space model
% Input: G, sys
% Example 1: [z, gain] = tzero(G)
% Example 2: [z, gain] = tzero(sys)
% Author: Daniel Mårtensson, 2017 Oktober

function [z, gain] = tzero(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get necessary info
		A = varargin{1}.A;
		sizeOrginalA = size(A, 2);
		B = varargin{1}.B;
		C = varargin{1}.C;
		D = varargin{1}.D;
		% Increase A at maximum size
		column = [size(B,2) size(C,2) size(D,2)];
		row = [size(B,1) size(C,1) size(D,1)];
		A = [A zeros(size(A,1), max(column) - size(A,2))];
		A = [A; zeros(max(row) - size(A,1), size(A,2))];
		% Make sure so matrix B, C, D have the same size as A
		C = rechange(A,C);
		B = rechange(A,B);
		D = rechange(A,D);
		% Compute the zeros
		M = [A B; C D];
		I = [eye(size(A,1), size(A,2)) B*0; C*0 D*0];
		z = eig(M, I);
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
		error('Only for state space models!')
	else
		error('This is not TF or SS');
	end

end
% Make sure so matrix B, C, D have the same size as A
function [matrix] = rechange(A, mat)
	if(size(A, 1) - size(mat, 1) >= 0)
		matrix = [mat; zeros(size(A, 1) - size(mat, 1), size(mat,2))];
	end
	if(size(A, 2) - size(matrix, 2) >= 0)
		matrix = [matrix zeros(size(matrix,1), size(A, 2) - size(matrix, 2))];
	end
end
