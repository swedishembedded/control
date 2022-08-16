% Filtfilt2 - A filter with no or less phase shifting. Very simple to use.
% Input: y(noisy signal), t(time signal), K(optional filter factor)
% Output: y(clean signal)
% Example 1: [y] = filtfilt2(y, t);
% Example 2: [y] = filtfilt2(y, t, K);
% Author: Daniel Mårtensson, April 2020
% Update: 27 April 2020 for MIMO signals
% Update: 29 April 2020 for Euler method 

function [y] = filtfilt2(varargin)
  % Check if there is any input
  if(isempty(varargin))
    error('Missing imputs')
  end
  
  % Get input
  if(length(varargin) >= 1)
    y = varargin{1};
  else
    error('Missing input y')
  end
  
  % Get time
  if(length(varargin) >= 2)
    t = varargin{2};
  else
    error('Missing input t')
  end
  
  % Get filter factor
  if(length(varargin) >= 3)
    K = varargin{3};
  else
    K = 0.1;
  end
  
  % Find size of y
  m = size(y, 1);
  n = size(y, 2);
  
  for i = 1:m
    % Simulate the noisy signal
    y1 = simulation(K, y(i, 1:n), t);
  
    % Flip
    y2 = flip(y1);
  
    % Run the simulation again
    y3 = simulation(K, y2, t);
  
    % Flip - Done
    y4 = flip(y3);
    
    % Save it
    y(i, 1:n) = y4;
  end
  
end

% Euler method for simple ODE
function [y] = simulation(K, y, t);
  h = t(2)-t(1); % Time step
  x = y(1); % Initial state
  for i = 1:length(t)
    x = x + h*(-1/K*x + 1/K*y(i));
    y(i) = x; % Save
  end
end
