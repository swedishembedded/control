% Observer Kalman Filter Identification
% Input: u(inputs), y(outputs), t(time vector), sampleTime(sample time)
% Output: sysd(Discrete state space model), K(Kalman gain matrix)
% Example: [sysd, K] = okid(u, y, t, sampleTime);
% Author: Daniel Mårtensson, Juli 2020

function [sysd, K] = okid(varargin)
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
  
  % Get time
  if(length(varargin) >= 3)
    t = varargin{3};
  else
    error('Missing time')
  end
  
  % Get sample time
  if(length(varargin) >= 4)
    sampleTime = varargin{4};
  else
    error('Missing sample time')
  end
  
  % Compute derivatives by using (y(i+1) - y(i)) / sampleTime
  derivatives = (outputs(:, 2:end) - outputs(:, 1:end-1))/sampleTime;
  outputs = outputs(:, 1:end-1);
  inputs = inputs(:, 1:end-1);
  t = t(1, 1:end-1);
  derivatives = derivatives';
  
  % Do error checking between outputs and inputs
  if(size(outputs, 2) ~= size(inputs, 2))
    error('Outputs and inputs need to have the same length of columns - Try transpose')
  end
  
  % Flip them so they are standing "up"
  Y = outputs';
  U = inputs';
  
  % Find the linear solution Ax = b
  X = linsolve([Y U], derivatives)'; % Important with transpose
  
  % Create the state space model
  m = size(Y, 2);
  A = X(:, 1:m);
  n = size(U, 2);
  B = X(:, m+1:m+n);
  sys = ss(0, A, B); % 0 = delay
  sysd = c2d(sys, sampleTime);
  
  % Find the kalman gain matrix K
  y = lsim(sysd, inputs, t);
  y = y(:, 1:2:end); % Remove the discrete shape
  close
  noise = outputs - y;
  Q = sysd.C'*sysd.C; % This is a standard way to select Q matrix for a kalman filter C'*C
  R = cov(noise'); % Important with transpose
  [K] = lqe(sysd, Q, R);
end
