% SPDX-License-Identifier: MIT
% Use linear Model Predictive Control
% Input: sysd(Discrete state space model), N(Horizon number), R(Reference
% vector), T(End time), a(lambda parameter), I(integral parameter 0 to 1),
% x0(Initial state, optimal)
% Output: y(Output signal), T(Discrete time vector), X(State vector), U(Output
% signal)
% Example 1: [Y, T, X, U] = lmpc(sysd, N, R, T)
% Example 2: [Y, T, X, U] = lmpc(sysd, N, R, T, a)
% Example 3: [Y, T, X, U] = lmpc(sysd, N, R, T, a, I)
% Example 4: [Y, T, X, U] = lmpc(sysd, N, R, T, a, I, x0)
% Author: Daniel Mårtensson
% Update: Replaced QP solver with LP solver due to CControl library has a C
% linear programming code as well
% Update: Added regularization and integral action

function [Y, T, X, U] = lmpc(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error ('Missing model')
	endif

	% Get model type
	type = varargin{1}.type;
	% Check if there is a TF or SS model
	if(strcmp(type, 'SS' ))
		% Get A, B, C, D matrecies
		sys = varargin{1};
		% Check if the model is discrete! MPC can only be discrete in this case.
		if sys.sampleTime <= 0
			error('Only discrete state space models');
		end
		A = sys.A;
		B = sys.B;
		C = sys.C;
		D = sys.D;
		delay = sys.delay;
		sampleTime = sys.sampleTime;

		% Get the horizon number
		if(length(varargin) >= 2)
			N = varargin{2};
		else
			error('Missing the horizon number');
		end

		% Get the reference vector R
		if(length(varargin) >= 3)
			R = repmat(varargin{3}, N, 1);
		else
			error('Missing the reference vector R');
		end

		% Get the total time
		if(length(varargin) >= 4)
			t = 0:sampleTime:varargin{4};
		else
			error('Missing the end time T');
		end

		% Get the lambda
		if(length(varargin) >= 5)
			a = varargin{5};
		else
			a = 0;
		end

		% Get the integral action parameter
		if(length(varargin) >= 6)
			I = varargin{6};
		else
			I = 0; % Max integral action
		end

		% Get the initial trajectory vector x
		if(length(varargin) >= 7)
			x = varargin{7};
		else
			x = zeros(size(A, 1), 1);
		end

		% Check if the system has integration behaviour already
		abseigenvalues = abs(pole(sys));
		if(max(abseigenvalues) >= 1)
			% Integral action is added already
			PHI = phiMat(A, C, N);
			GAMMA = gammaMat(A, B, C, N);
		else
			% Add integral action - It's very good and pratical!
			% A = [A B; 0 I]
			% B = [0; I]
			% C = [C 0]
			% x = [x; u(k-1)]
			am = size(A, 1);
			bn = size(B, 2);
			cm = size(C, 1);
			A = [A B; zeros(bn, am) eye(bn, bn)];
			B = [zeros(am, bn); eye(bn, bn)];
			C = [C zeros(cm, bn)];
			x = [x; zeros(bn, 1)];
		end

		% Check if it's MATLAB or Octave
		isOctave = exist('OCTAVE_VERSION', 'builtin') ~= 0;

		% Find the observability matrix PHI and lower triangular toeplitz matrix GAMMA of C*PHI
		PHI = phiMat(A, C, N);
		GAMMA = gammaMat(A, B, C, N);

		% Solve: R = PHI*x + GAMMA*U with linear programming: Max: c^Tx, S.t: Ax <= b, x >= 0
		% clp = (A'*A + lambda)'*b
		% blp = A'*b
		% alp = A'*A + lambda
		lambda = a*eye(size(GAMMA));
		clp = (GAMMA'*GAMMA + lambda)'*(R - PHI*x);
		blp = GAMMA'*(R - PHI*x);
		alp = GAMMA'*GAMMA + lambda;

		% Settings for the GLPK for GNU Octave only
		iteration_limit = 200;
		CTYPE = repmat("U", 1, N*size(B,2));
		VARTYPE = repmat("C", 1, N*size(B,2));

		% Loop and save the data
		past_inputs = zeros(1, size(B, 2));
		delta = zeros(1, size(B, 2));
		for k = 1:length(t)
			% Return states and input signals
			X(:,k) = x;
			U(:,k) = delta; % First element!

			% Compute outputs
			Y(:,k) = C*x + D*delta; % size(B, 2) = If multiple inputs...

			% Update states
			% For applying this MPC regulator in reality, then state vector x should be the "sensors" if C = identity and D = 0
			x = A*x + B*delta;

			% Update the constraints and objective function
			clp = (GAMMA'*GAMMA + lambda)'*(R - PHI*x);
			blp = GAMMA'*(R - PHI*x);

			% Linear programming
			if(isOctave == 1)
				u = glpk (clp, alp, blp, [], [], CTYPE, VARTYPE, -1);
			else
				u = linprog2(clp, alp, blp, 0, iteration_limit); % Used for MATLAB users
			end

			% Do integral action by compare new inputs with old inputs
			delta = u(1:size(B, 2)) - I*past_inputs;
			past_inputs = u(1:size(B, 2));
		end

		% Change t and y vector and u so the plot look like it is discrete - Important!
		for(i = 1:2:length(Y)*2)
			leftPart = Y(:,1:i);
			rightPart = Y(:,(i+1):end);
			Y = [leftPart Y(:,i) rightPart];
		end

		for(i = 1:2:length(t)*2)
			leftPart = t(1:i);
			rightPart = t((i+1):end);
			t = [leftPart t(i) rightPart];
		end

		for(i = 1:2:length(U)*2)
			leftPart = U(:,1:i);
			rightPart = U(:,(i+1):end);
			U = [leftPart U(:,i) rightPart];
		end

		for(i = 1:2:length(X)*2)
			leftPart = X(:,1:i);
			rightPart = X(:,(i+1):end);
			X = [leftPart X(:,i) rightPart];
		end

		% Just remove the first one 
		T = t(:,2:length(t));
		% And the last one
		Y = Y(:,1:(length(Y)-1));
		% And for U and X too
		U = U(:,1:(length(U)-1));
		X = X(:,1:(length(X)-1));
		% Now we have two vectors which look like a discrete signal

		% Plot - How many subplots?
		for i = 1:size(C,1)
			subplot(size(C,1),1,i)
			plot(T, Y(i,:));
			ylabel(strcat('y', num2str(i)));
			if (sampleTime > 0)
				xlabel(strcat(num2str(sampleTime), ' time unit/sample'));
			else
				xlabel('Time units');
			end
			grid on
		end
	end
end

% This simplex method has been written as it was C code
function [x] = linprog2(c, A, b, max_or_min, iteration_limit)
	row_a = size(A, 1);
	column_a = size(A, 2);

	if(max_or_min == 0)
		% Maximization
		x = opti(c, A, b, row_a, column_a, max_or_min, iteration_limit);
	else
		% Minimization
		x = opti(b, A', c, column_a, row_a, max_or_min, iteration_limit);
	end
end


function [x] = opti(c, A, b, row_a, column_a, max_or_min, iteration_limit)

	% Clear the solution
	if(max_or_min == 0)
		x = zeros(column_a, 1);
	else
		x = zeros(row_a, 1);
	end

	% Create the tableau
	tableau = zeros(row_a + 1, column_a + row_a + 2);
	j = 1;
	for i = 1:row_a
		% First row
		tableau(i, 1:column_a) = A(i, 1:column_a);

		% Slack variable s
		j = column_a + i;
		tableau(i, j) = 1;

		% Add b vector
		tableau(i, column_a + row_a + 2) = b(i);
	end

	% Negative objective function
	tableau(row_a + 1, 1:column_a) = -c(1:column_a);
 
	% Slack variable for objective function
	tableau(row_a + 1, column_a + row_a + 1) = 1;

	% Do row operations
	entry = -1.0; % Need to start with a negative number because MATLAB don't have do-while! ;(
	pivotColumIndex = 0;
	pivotRowIndex = 0;
	pivot = 0.0;
	value1 = 0.0;
	value2 = 0.0;
	value3 = 0.0;
	smallest = 0.0;
	count = 0;
	while(entry < 0) % Continue if we have still negative entries
		% Find our pivot column
		pivotColumIndex = 1;
		entry = 0.0;
		for i = 1:column_a + row_a + 2
			value1 = tableau(row_a + 1, i);
			if(value1 < entry)
				entry = value1;
				pivotColumIndex = i;
			end
		end

		% If the smallest entry is equal to 0 or larger than 0, break
		if(or(entry >= 0.0, count >= iteration_limit))
			break;
		end

		% Find our pivot row
		pivotRowIndex = 1;
		value1 = tableau(1, pivotColumIndex); % Value in pivot column
		if(value1 == 0)
			value1 = eps;
		end
		value2 = tableau(1, column_a+row_a+2); % Value in the b vector
		smallest = value2/value1; % Initial smalles value1
		for i = 2:row_a
			value1 = tableau(i, pivotColumIndex); % Value in pivot column
			if(value1 == 0)
				value1 = eps;
			end
			value2 = tableau(i, column_a+row_a+2); % Value in the b vector
			value3 = value2/value1;
			if(or(and(value3 > 0, value3 < smallest), smallest < 0))
				smallest = value3;
				pivotRowIndex = i;
			end
		end

		% We know where our pivot is. Turn the pivot into 1
		% 1/pivot * PIVOT_ROW -> PIVOT_ROW
		pivot = tableau(pivotRowIndex, pivotColumIndex); % Our pivot value
		if(pivot == 0)
			pivot = eps;
		end
		for i = 1:column_a + row_a + 2
			value1 = tableau(pivotRowIndex, i); % Our row value at pivot row
			tableau(pivotRowIndex, i) = value1 * 1/pivot; % When value1 = pivot, then pivot will be 1
		end

		% Turn all other values in pivot column into 0. Jump over pivot row
		% -value1* PIVOT_ROW + ROW -> ROW
		for i = 1:row_a + 1
			if(i ~= pivotRowIndex)
				value1 = tableau(i, pivotColumIndex); %	This is at pivot column
				for j = 1:column_a+row_a+2
					value2 = tableau(pivotRowIndex, j); % This is at pivot row
					value3 = tableau(i, j); % This is at the row we want to be 0 at pivot column
					tableau(i, j) = -value1*value2 + value3;
				end
			end
		end

		% Count for the iteration
		count = count + 1;
	end

	% If max_or_min == 0 -> Maximization problem
	if(max_or_min == 0)
		% Now when we have shaped our tableau. Let's find the optimal solution. Sum the columns
		for i = 1:column_a
			value1 = 0; % Reset
			for j = 1:row_a + 1
				value1 = value1 + tableau(j, i); % Summary
				value2 = tableau(j, i); %	If this is 1 then we are on the selected

				% Check if we have a value that are very close to 1
				if(and(and(value1 < 1 + eps, value1 > 1 - eps), value2 > 1 - eps))
					x(i) = tableau(j, column_a+row_a+2);
				end
			end
		end
	else
		% Minimization (The Dual method) - Only take the bottom rows on the slack variables
		for i = 1:row_a
			x(i) = tableau(row_a+1, i + column_a); % We take only the bottom row at start index column_a
		end
	end
end


function PHI = phiMat(A, C, N)
	% Create the special Observabillity matrix
	PHI = [];
	for i = 1:N
		PHI = vertcat(PHI, C*A^i);
	end
end

function GAMMA = gammaMat(A, B, C, N)
	% Create the lower triangular toeplitz matrix
	GAMMA = [];
	for i = 1:N
		GAMMA = horzcat(GAMMA, vertcat(zeros((i-1)*size(C*A*B, 1), size(C*A*B, 2)),cabMat(A, B, C, N-i+1)));
	end

end

function CAB = cabMat(A, B, C, N)
	% Create the column for the GAMMA matrix
	CAB = [];
	for i = 0:N-1
		CAB = vertcat(CAB, C*A^i*B);
	end
end
