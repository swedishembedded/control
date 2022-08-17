% SPDX-License-Identifier: MIT
% Author: Daniel Mårtensson, October 2017
% Generate control law gain matrix L of a state space model by using eigen values vector P
% Input: G, sys, P
% Example 1: [L] = acker(sys, P)
% Source from Modern Control Engineering, Ogata, 3th edition.

function [L] = acker(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing input')
	end

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get model
		sys = varargin{1};
		% Get matrecies
		A = sys.A;
		B = sys.B;

		% Check if B is SISO
		if(size(B, 2) > 1)
			error('Model must be a SISO model!')
		end

		% Get eigen values
		if(length(varargin) >= 2)
			P = varargin{2};
		else
			error('Missing the closed loop poles')
		end

		% Vectorize P
		P = P(:);
		if(size(A,1) ~= length(P))
			error('Poles need to have the same dimension as matrix A')
		end

		% Create the control law gain matrix L
		%Formula from Ogata Modern Control Engineering
		Cm = ctrb(sys); % Controllability matrix
		RealPoly = real(poly(P)); % Real polynomal of P
		L = Cm\polyvalm(RealPoly, A);
		L = L(size(A,2),:);

		% Check if the user has put in very bad pole locations
		P = sort(P);
		nonZeroPoles = find(P ~= 0);
		P = P(nonZeroPoles);
		% Sort the eigen values
		eigenvalues = sort(eig(A-B*L));
		% Ge the egenvalues which has non zero poles
		eigenvalues = eigenvalues(nonZeroPoles);
		% Get the absolute maximum value of P
		M = abs(P);

		% Get the difference between pole locations and eigen values
		Diff = abs(P-eigenvalues);
		if(max(Diff./M) > .05)
			disp('Warning: Pole locations are in more that 5% error')
		end

	elseif(strcmp(type, 'TF' ))
		error('Transfer function models not supported. Convert to state space!')
	else
		error('This is not TF or SS');
	end
end
