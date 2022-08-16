% Do a nonlinear simulation of a nonlinear state space model
% Input: model, u(inputs), x0(initial state vector), stepTime, simulationMethod('ode23', 'ode45', 'ode15s', 'ode23s'), options
% Example 1: [x,t] = nlsim(model, u, x0, stepTime);
% Example 2: [x,t] = nlsim(model, u, x0, stepTime, 'ode23');
% Example 3: [x,t] = nlsim(model, u, x0, stepTime, 'ode23', options);
% Author: Daniel Mårtensson, April 2022

function [x,t] = nlsim(varargin)
  % Check if there is any input
  if(isempty(varargin))
    error('Missing inputs')
  end
  
  % Get model
  if(length(varargin) >= 1)
    model = varargin{1};
  else
    error('Missing model')
  end
  
  % Get inputs
  if(length(varargin) >= 2)
    u = varargin{2};
  else
    error('Missing inputs')
  end
  
  % Get initial state vector
  if(length(varargin) >= 3)
    x0 = varargin{3};
  else
    error('Missing initial state vector')
  end
  
  % Get stepTime
  if(length(varargin) >= 4)
    stepTime = varargin{4};
  else
    error('Missing sample time')
  end
  
  % Get simulation method
  if(length(varargin) >= 5)
    simulationMethod = varargin{5};
  else
    simulationMethod = 'ode45';
  end
  
  % Get options 
  if(length(varargin) >= 6)
    options = varargin{6};
  else
    options = odeset();
  end
  
  % Savings 
  L = length(u);
  x = zeros(size(x0, 1), L);
  t = zeros(1, L);
 
  % Loop
  for i = 1:L
    % Save time and output 
    x(:, i) = x0;
    t(i) = stepTime*(i-1);
    
    % Simulate ode45, ode23, ode15s, ode23s
    tspan = [stepTime*(i-1) stepTime*i];
    if(strcmp('ode45', simulationMethod))
      [time, output] = ode45(@(t, x) model(t, x, u(:, i)), tspan, x0, options);
    elseif(strcmp('ode23', simulationMethod))
      [time, output] = ode23(@(t, x) model(t, x, u(:, i)), tspan, x0, options);
    elseif(strcmp('ode15s', simulationMethod))
      [time, output] = ode15s(@(t, x) model(t, x, u(:, i)), tspan, x0, options);
    elseif(strcmp('ode23s', simulationMethod))
      [time, output] = ode23s(@(t, x) model(t, x, u(:, i)), tspan, x0, options);
    else
      error('Choose simulation method such as ode45, ode23, ode15s, ode23s');
    end 
    
    % Next state 
    x0 = output(end, :)';
  end
  
  % This is for the sub plot - How many max rows should we have
  rows = max(size(x,1), size(u, 1));
  
  % Plot - How many subplots?
  for i = 1:size(x,1)
    subplot(rows,1,i)
    plot(t, x(i,:));
    ylabel(strcat('x', num2str(i)));
    xlabel(strcat(num2str(stepTime), ' time unit/step time'));
    grid on
  end

  % Plot the input signals as well
  for i = 1:size(u, 1)
    subplot(rows, 1, i)
    hold on
    plot(t, u(i, :))
    grid on
    if(i <= size(x,1))
      legend(strcat('x', num2str(i)), strcat('u', num2str(i)))
    else
      legend(strcat('u', num2str(i)))
    end
  end
  
end