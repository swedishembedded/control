% SPDX-License-Identifier: MIT
% Sparse Identification of Nonlinear Dynamics
% Activations for e.g u and y e.g: 1, u, y, u^2, y^2, u^3, y^3, u*y, sin(u), sin(y), cos(u), cos(y), tan(u), tan(y), sqrt(u), sqrt(y)
% Input: inputs, output, derivatives, activations, lambda
% Example: [dx] = sindy(inputs, outputs, activations, lambda, sampleTime);
% Author: Daniel Mårtensson, May 2, 2020
% Update: Added more error handling and now display which activation function that being used, June 25, 2020

function [dx] = sindy(varargin)
	% Check if there is any input
	if(isempty(varargin))
		error('Missing inputs')
	end

	% Get inputs
	if(length(varargin) >= 1)
		inputs = varargin{1};
	else
		error('Missing inputs')
	end

	% Get outputs
	if(length(varargin) >= 2)
		outputs = varargin{2};
	else
		error('Missing outputs')
	end

	% Get activations
	if(length(varargin) >= 3)
		activations = varargin{3};
	else
		error('Missing activations')
	end

	% Get lambda
	if(length(varargin) >= 4)
		lambda = varargin{4};
	else
		error('Missing lambda')
	end

	% Get sampleTime
	if(length(varargin) >= 5)
		sampleTime = varargin{5};
	else
		error('Missing sample time')
	end

	% Find the derivative of outputs
	derivatives = (outputs(2:end, :)-outputs(1:end-1, :))/sampleTime;

	% Same length as derivatives
	outputs = outputs(1:end-1, :);
	inputs = inputs(1:end-1, :);

	% Transpose them all
	inputs = inputs';
	outputs = outputs';
	derivatives = derivatives;

	% Do error checking between outputs and inputs
	if(size(outputs, 2) ~= size(inputs, 2))
		error('States and inputs need to have the same length of columns - Try transpose')
	end

	% Do error checking between derivatives and inputs
	if(size(derivatives, 1) ~= size(inputs, 2))
		error('Derivatives and inputs need to have the same length - Try transpose')
	end

	% Create variables x(1), x(2), x(3), ... , u(1), u(2), u(3) ...
	variables = "";
	for i = 1:size(outputs, 1)
		variables = [variables; 'x(', num2str(i), ')'];
	end
	for i = 1:size(inputs, 1)
		variables = [variables; 'u(', num2str(i), ')'];
	end


	% Create our data, it must contain outputs and inputs. States can be interpreted as outputs by the way!
	data = [outputs; inputs];

	% Create our O matrix - Our system matrix. Here we create our data by the candidate functions
	l = size(data, 1);
	labels = [""]; % String array - Important so we can see which candidate we have selected
	O = zeros(length(data), 1);
	columnposition = 2; % We start at 2 due to the first column is constant 1
	O(:, 1) = 1; % Add ones only as constants
	labels = [labels; "1"];
	[O, columnposition, labels] = candidate(O, data, l, columnposition, variables, labels, "none"); % We add every inputs
	[O, columnposition, labels] = candidate(O, data.^2, l, columnposition, variables, labels, "^2"); % We use the raised to power 2 candidate
	[O, columnposition, labels] = candidate(O, data.^3, l, columnposition, variables, labels, "^3"); % We use the raised to power 3 candidate
	[O, columnposition, labels] = candidate(O, data.^4, l, columnposition, variables, labels, "^4"); % We use the raised to power 4 candidate
	[O, columnposition, labels] = candidate(O, data.^5, l, columnposition, variables, labels, "^5"); % We use the raised to power 5 candidate
	[O, columnposition, labels] = candidate(O, data.^5, l, columnposition, variables, labels, "^5");
	[O, columnposition, labels] = candidate(O, data.^6, l, columnposition, variables, labels, "^6");
	[O, columnposition, labels] = candidate(O, data.^7, l, columnposition, variables, labels, "^7");
	[O, columnposition, labels] = candidate(O, data.^8, l, columnposition, variables, labels, "^8");
	[O, columnposition, labels] = candidate(O, data.^9, l, columnposition, variables, labels, "^9");
	[O, columnposition, labels] = candidate(O, data.^10, l, columnposition, variables, labels, "^10");
	[O, columnposition, labels] = candidate(O, data.^11, l, columnposition, variables, labels, "^11");
	[O, columnposition, labels] = candidate(O, data.^12, l, columnposition, variables, labels, "^12");
	[O, columnposition, labels] = candidate(O, data.^13, l, columnposition, variables, labels, "^13");
	[O, columnposition, labels] = candidate(O, data.^14, l, columnposition, variables, labels, "^14");
	[O, columnposition, labels] = candidate(O, data.^15, l, columnposition, variables, labels, "^15");
	[O, columnposition, labels] = candidatexyz(O, data, l, columnposition, variables, labels, "*"); % We add e.g x*y, z*u, y*u
	[O, columnposition, labels] = candidatexyz(O, data, l, columnposition, variables, labels, "^2"); % We add e.g x^2*y^2, z^2*u^2, y^2*u^2
	[O, columnposition, labels] = candidatexyz(O, data, l, columnposition, variables, labels, "sin"); % We add e.g sin(x*y), sin(z*u), sin(y*u)
	[O, columnposition, labels] = candidate(O, sin(data), l, columnposition, variables, labels, "sin"); % We use the add sin candidate
	[O, columnposition, labels] = candidate(O, cos(data), l, columnposition, variables, labels, "cos"); % We use the add cos candidate
	[O, columnposition, labels] = candidate(O, tan(data), l, columnposition, variables, labels, "tan"); % We use the add tan candidate
	[O, columnposition, labels] = candidate(O, sqrt(data), l, columnposition, variables, labels, "sqrt"); % We use the add sqrt candidate
	[O, columnposition, labels] = candidate(O, exp(data), l, columnposition, variables, labels, "exp"); % We use the add exp candidate
	[O, columnposition, labels] = candidate(O, log(data), l, columnposition, variables, labels, "log"); % We use the add log candidate
	% You can add more candidates here!

	% Cut O matrix so we only using the selected candidates
	lengthActivations = length(activations);
	lengthO = size(O, 2);
	if(lengthActivations ~= lengthO)
		error(strcat('Try to have activations as [', num2str(ones(1, lengthO)), '] to begin with'));
	end
	O = O(:, 1:lengthActivations);

	% Delete all zero columns when we activate the candidates - Now we only using selected candidates
	O = O.*activations;
	O( :, ~any(O,1) ) = [];

	% Print candidates that we are using
	text = strcat('Candidates we are using with selected activations for:', '[', num2str(activations), ']');
	disp(text);
	used_labels = [""];
	for j = 1:length(activations)
		if(activations(j) ~= 0)
			used_labels = [used_labels; labels(j, :)]; % Save them for the last for-loop
		end
		disp(strcat(num2str(activations(j)), ':', labels(j, :))) % Show enabled and disabled
	end

	% Do least squares for every column of derivatives - This is the heart of SINDy
	state_dimension = size(O, 2);
	E = stls_regression(O, derivatives, lambda);

	% Create function handler
	handler = "@(t, x, u)";

	% Print E - Don't replace " with '. They have a pourpose.
	disp("\nOur nonlinear state space model:");
	equations = "";
	for i = 1:size(E, 2)
		% This is for so we don't write + directly after dy =
		firstTimeWriting = 1;
		% Save the equations here
		equation = [];
		% This is the right side of the equation
		for j = 1:size(used_labels, 1)
			% Only select values that have been sorted out by lambda value
			if(E(j, i) ~= 0)
				if(and(E(j, i) > 0, firstTimeWriting == 0))
					val = sprintf(" + %f", E(j, i));
				elseif(and(E(j, i) < 0, firstTimeWriting == 0))
					val = sprintf(" - %f", abs(E(j, i)));
				else
					if(E(j, i) > 0)
						val = sprintf(" %f", E(j, i));
					else
						val = sprintf(" -%f", abs(E(j, i)));
					end
				end
				firstTimeWriting = 0;
				equation = strcat(equation, val, '*', used_labels(j, :));
			end
		end
		% Add ; if i is below size of E
		if(i < size(E, 2))
			equation = strcat(equation, ';');
		end
		% Add equation to equations
		equations = [equations equation];
	end

	% Create the nonlinear model
	dx = str2func(strcat(handler, '[', equations , ']'))

end

function dXi = stls_regression(O, dX, lambda)

	% Initial guess
	dXi = linsolve(O, dX);
	state_dimension = size(dX, 2);

	% lambda is our sparsification knob.
	for k=1:10
			smallinds = (abs(dXi)<lambda);			% Find small coefficients
			dXi(smallinds) = 0;								 % Turn small numbers into zeros
			for ind = 1:state_dimension				 % For every state dimension
				biginds = ~smallinds(:,ind);
				dXi(biginds,ind) = linsolve(O(:,biginds),dX(:,ind)); % Regress dynamics onto remaining terms to find sparse Xi
			end
	end
end

% This add its candidates
function [O, columnposition, labels] = candidate(O, data, l, columnposition, variables, labels, func)
	for i = 1:l

		% Add the name of the label
		switch func
			case 'none'
				labels = [labels; variables(i, :)]; % Regular
			case '^2'
				labels = [labels; strcat(variables(i, :), '^2')]; % e.g x^2
			case '^3'
				labels = [labels; strcat(variables(i, :), '^3')]; % e.g x^3
			case '^4'
				labels = [labels; strcat(variables(i, :), '^4')]; % e.g x^4
			case '^5'
				labels = [labels; strcat(variables(i, :), '^5')]; % e.g x^5
			case '^6'
				labels = [labels; strcat(variables(i, :), '^6')]; % e.g x^6
			case '^7'
				labels = [labels; strcat(variables(i, :), '^7')]; % e.g x^7
			case '^8'
				labels = [labels; strcat(variables(i, :), '^8')]; % e.g x^8
			case '^9'
				labels = [labels; strcat(variables(i, :), '^9')]; % e.g x^9
			case '^10'
				labels = [labels; strcat(variables(i, :), '^10')]; % e.g x^10
			case '^11'
				labels = [labels; strcat(variables(i, :), '^11')]; % e.g x^11
			case '^12'
				labels = [labels; strcat(variables(i, :), '^12')]; % e.g x^12
			case '^13'
				labels = [labels; strcat(variables(i, :), '^13')]; % e.g x^13
			case '^14'
				labels = [labels; strcat(variables(i, :), '^14')]; % e.g x^14
			case '^15'
				labels = [labels; strcat(variables(i, :), '^15')]; % e.g x^15
			case 'sin'
				labels = [labels; strcat('sin(', variables(i, :), ')')]; % e.g sin(x)
			case 'cos'
				labels = [labels; strcat('cos(', variables(i, :), ')')];
			case 'tan'
				labels = [labels; strcat('tan(', variables(i, :), ')')];
			case 'sqrt'
				labels = [labels; strcat('sqrt(', variables(i, :), ')')];
			case 'exp'
				labels = [labels; strcat('exp(', variables(i, :), ')')];
			case 'log'
				labels = [labels; strcat('log(', variables(i, :), ')')];
			% Add more cases here...
		end

		% Add to O matrix
		O(:, columnposition) = data(i,:);
		columnposition = columnposition + 1;
	end
end

% This add candidates with multiplication about each other
function [O, columnposition, labels] = candidatexyz(O, data, l, columnposition, variables, labels, func)
	for j = 1:l
		for i = 1:l
			if(j ~= i)

				% Add the name of the label and add to O matrix
				switch func
					case '*'
						labels = [labels; strcat(variables(j, :), '*', variables(i, :))]; % e.g x*y
						O(:, columnposition) = data(j,:).*data(i,:); % e.g x.*y, y.*z, z.*k, k.*j, j.*i where inputs is [x;y;z,k,j,i] and l is 6 due to row size of [x;y;z,k,j,i]
					case '^2'
						labels = [labels; strcat(variables(j, :), '^2*', variables(i, :), '^2')]; % e.g x^2*y^2
						O(:, columnposition) = (data(j,:).^2).*(data(i,:).^2);
					case 'sin'
						labels = [labels; strcat('sin(', variables(j, :), '*', variables(i, :), ')')]; % e.g sin(x*y)
						O(:, columnposition) = sin(data(j,:).*data(i,:));
					% Add more cases here...
				end

				% Add to O matrix
				columnposition = columnposition + 1;
			end
		end
	end
end
