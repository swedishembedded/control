% SPDX-License-Identifier: MIT
% Do a nonlinear simulation of a transfer function or a state space model
% Here you can use saturation limits on inputs
% Input: sys, u, t, x0(optional), xL(optional)
% Example 1: [y,t,x] = satlsim(sys,u,t,x0, xL)
% Author: Daniel Mårtensson, January 2018


function [y,t,X] = satlsim(varargin)
	% Check if there is some input arguments or it's not a model
	if(isempty(varargin{1}))
		error ('Missing model')
	end

	if(strcmp(varargin{1}.type,'SS'))
		% Get A, B, C, D and delay
		model = varargin{1};
		A = model.A;
		B = model.B;
		C = model.C; % Will not be used
		D = model.D; % Will not be used
		delay = model.delay;
		sampleTime = model.sampleTime;

		% Get input
		u = varargin{2}; % In signal vector
		% Check in signal vector
		if(size(u, 1) ~= size(B, 2))
			error('In signal vecor has not the same columns as B matrix')
		end

		% Get time
		t = varargin{3};
		% Check t vector
		if(size(t, 1) > 1)
			error('No double time vector are allowed')
		end

		% Check if t and u have the same length
		lengthTime = length(t);
		lengthSignal = length(u);
		if(lengthTime ~= lengthSignal)
			error('Input and time has not the same length')
		end

		% Get the initial state vector
		if(length(varargin) >= 4)
			x = varargin{4}(:); % Initial state vector
			if(size(x, 1) ~= size(A, 1))
				error('Initial state vector vecor has not the same rows as A matrix')
			end
		else
			% Does not exist - create one then!
			x = zeros(size(A, 1), 1); % Assume x0 = [0; 0; 0; ..... ; 0]
		end

		% Get the saturation xL limit matrix
		if(length(varargin) >= 5)
			xL = varargin{5}; % eg. [-1 1; -2 3; ... -n +m]
			% Check if the xL has the same dimension as A
			if size(xL, 1) ~= size(A, 1)
					error('The saturation xL matrix has not the same dimension as matrix A')
			end
			% Check if the xL has the length 2
			if size(xL, 2) ~= 2
				error('The saturation xL matrix has not the length 2')
			end
			% Enable
			satXEnable = 'yes';
		else
			satXEnable = 'no';
		end


		% Create discrete model - Important for simulation
		if(sampleTime > 0) % Allready Discrete!
			sysd = varargin{1};
		else % Time continous
			sysd = c2d(varargin{1}, t(2) - t(1)); % Sample time is the difference t(2) - t(1)
		end

		% Discrete matrecies
		Ad = sysd.A;
		Bd = sysd.B;
		Cd = sysd.C;
		Dd = sysd.D;

		if strcmp(satXEnable, 'yes')
			% Simulation
			for k = 1:size(t,2)

				% Just in case if we start at initial states who are above xL limit saturation
				x = sat(xL, x);

				X(:,k) = x; % The return states
				y(:,k) = Cd*x + Dd*u(:,k);
				x = Ad*x + Bd*u(:,k); % Update state vector

				% Do a saturation for state vector x
				x = sat(xL, x);
			end
		else
			% Simulation
			for k = 1:size(t,2)

				X(:,k) = x; % The return states
				y(:,k) = Cd*x + Dd*u(:,k);
				x = Ad*x + Bd*u(:,k); % Update state vector
			end
		end


		% If we have a sample time bigger that 0, then we need to make sure
		% that the signal look like it's in discrete form.
		% If the model has the sample time t(2) - t(1)
		% the model will be inteprented as time continous
		if(sampleTime > 0)
			% Change t and y vector so the plot look like it is discrete
			for(i = 1:2:length(y)*2)
				leftPart = y(:,1:i);
				rightPart = y(:,(i+1):end);
				y = [leftPart y(:,i) rightPart];
			end

			for(i = 1:2:length(t)*2)
				leftPart = t(1:i);
				rightPart = t((i+1):end);
				t = [leftPart t(i) rightPart];
			end

			% Just remove the first one 
			t = t(:,2:length(t));
			% And the last one
			y = y(:,1:(length(y)-1));
			% Now we have two vectors which look like a discrete signal
		end

		% Plot - How many subplots?
		for i = 1:size(C,1)
			subplot(size(C,1),1,i)
			plot(t, y(i,:));
			ylabel(strcat('y', num2str(i)));
			if (sampleTime > 0)
				xlabel(strcat(num2str(sampleTime), ' time unit/sample'));
			else
				xlabel('Time units');
			end
			grid on
		end

	elseif(strcmp(varargin{1}.type,'TF'))
		% TF to SS
		sys = tf2ss(varargin{1}, 'OCF');

		% Get input
		u = varargin{2}; % In signal vector
		t = varargin{3}; % Time vector

		% Check the initial state vector
		if(length(varargin) >= 4)
			x0 = varargin{4}(:); % Initial state vector
			if(size(x0, 1) ~= size(sys.A, 1))
				error('Initial state vector vecor has not the same rows as A matrix')
			end
		else
			x0 = zeros(size(sys.A, 1), 1); % Assume x0 = 0
		end

		% Call lsim
		[y,t,X] = nlsim(sys, u, t, x0);

	else
		error('No transfer function or state space model')
	end

end

% Saturation for state x vector
function val = sat(limitMatrix, inputVector)
	% First create the value vector who can hold values
	val = zeros(size(inputVector)); % always an nx1 vector

	for i = 1:size(val, 1)
		if inputVector(i) <= limitMatrix(i, 1)
			val(i) = limitMatrix(i, 1);
		elseif inputVector(i) >= limitMatrix(i, 2)
			val(i) = limitMatrix(i, 2);
		else
			val(i) = inputVector(i);
		end
	end

end
