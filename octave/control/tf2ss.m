% SPDX-License-Identifier: MIT
% Transform transfer function to state space model
% Input: G, canonical form(optional)
% Example 1: sys = tf2ss(G) % Observable canonical form is used as default
% Example 2: sys = tf2ss(G, 'OCF') % Observable canonical form
% Example 3: sys = tf2ss(G, 'CCF') % Controllable canonical form
% Author: Daniel Mårtensson, 2017 September

function [sys] = tf2ss(varargin)
	% Check if there is a MIMO TF model
	if(or(size(varargin{1}, 1) > 1, size(varargin{1}, 2) > 1))
		error('Only for SISO tranfer functions!')
	end

	% Check if there is some input arguments or it's not a transfer function
	if(or(isempty(varargin), ~strcmp(varargin{1}.type, 'TF' )))
		error ('Missing transfer function')
	end

	% Get necessary info
	numerator = varargin{1}.num;
	denomerator = varargin{1}.den;
	delay = varargin{1}.delay;

	% Check if the numerator is longer that denomerator
	if(length(numerator) > length(denomerator))
		error('Numerator cannot be longer that denomerator - No existing state space model');
	end

	% Make sure that the highest order element in the denomerator has the scalar 1 - Proper transfer function
	% Divide all the elements in numerator and denomerator with denomerator(1)
	numerator = numerator./(denomerator(1));
	denomerator = denomerator./(denomerator(1));

	% Make sure that the length of the numerators is equal to the length of denomerator
	if(length(numerator) < length(denomerator))
		% Add 0's at the numerator
		numerator = [zeros(1, length(denomerator) - length(numerator)) numerator];
		% Check if form is CCF or OCF
		if(length(varargin) >= 2)
			if(strcmp(varargin{2},'CCF'))
				[A, B, C, D] = controllable_canonical_form(numerator,denomerator);
			elseif(strcmp(varargin{2},'OCF'))
				[A, B, C, D] = observable_canonical_form(numerator,denomerator);
			else
				txt = sprintf('Warning: Does not reginoze the canonical form %s. Observable canonical form is used\n', varargin{2});
				disp(txt);
				[A, B, C, D] = observable_canonical_form(numerator,denomerator);
			end
		else
			txt = sprintf('Converting from transfer function to state space\nObservable canonical form is used as default\n');
			disp(txt);
			[A, B, C, D] = observable_canonical_form(numerator,denomerator);
		end
	else
		% They are equal - No add 0's from the start
		% Check if form is CCF or OCF
		if(length(varargin) >= 2)
			if(strcmp(varargin{2},'CCF'))
				[A, B, C, D] = controllable_canonical_form(numerator,denomerator);
			elseif(strcmp(varargin{2},'OCF'))
				[A, B, C, D] = observable_canonical_form(numerator,denomerator);
			else
				txt = sprintf('Warning: Does not reginoze the canonical form %s. Observable canonical form is used\n', varargin{2});
				disp(txt);
				[A, B, C, D] = observable_canonical_form(numerator,denomerator);
			end
		else
			txt = sprintf('Converting from transfer function to state space\nObservable canonical form is used as default\n');
			disp(txt);
			[A, B, C, D] = observable_canonical_form(numerator,denomerator);
		end
	end

	% Check if A, B, C, D matrecis contains nothing. Give them zero then!
	if(or(size(A,1) == 0, size(A,2) == 0))
		A = [0];
	end

	if(or(size(B,1) == 0, size(B,2) == 0))
		B = [0];
	end

	if(or(size(C,1) == 0, size(C,2) == 0))
		C = [0];
	end

	if(or(size(D,1) == 0, size(D,2) == 0))
		D = [0];
	end


	% Now everything is done!
	sys = ss(delay, A, B,C, D);
	sys.sampleTime = varargin{1}.sampleTime;
end

% This function generates matrix A, B, C, D from observable canonical form
function [A, B, C, D] = observable_canonical_form(numerator,denomerator)
	% Matrix D
	D = numerator(1); % If numerator == denomerator ---> D > 0, else D = 0
	% Matrix B - Notice that "- denomerator(1,2:(length(denomerator)))'*D" are included because the D
	B = [numerator(1,2:(length(numerator)))' - denomerator(1,2:(length(denomerator)))'*D];
	% Matrix C
	C = [1 zeros(1, size(B, 1) - 1)]; % Minus -1 because we add 1
	% Matrix A
	A1 = -denomerator(1,2:(length(denomerator)))';
	A2 = eye(size(B, 1) - 1);
	A3 = zeros(size(C', 1) - 1, 1)';
	A4 = [A2; A3];
	A = [A1 A4];
end

% This function generates matrix A, B, C, D from controllable canonical form
function [A, B, C, D] = controllable_canonical_form(numerator,denomerator)
	% Matrix D
	D = numerator(1); % If numerator == denomerator ---> D > 0, else D = 0
	% Matrix C - Notice that "- denomerator(1,2:(length(denomerator)))'*D" are included because the D
	C = [numerator(1,2:(length(numerator)))' - denomerator(1,2:(length(denomerator)))'*D]';
	% Matrix B
	B = [1 zeros(1, size(C, 2) - 1)]'; % Minus -1 because we add 1
	% Matrix A
	A1 = -denomerator(1,2:(length(denomerator)));
	A2 = eye(size(B, 1) - 1);
	A3 = zeros(size(C', 1) - 1, 1);
	A4 = [A2 A3];
	A = [A1; A4];
end
