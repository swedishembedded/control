% Square Root Unscented Kalman Filter For State Estimation (A better version than regular UKF)
% Input: Se list below
% Output: S(Parameter estimate error covariance), xhat(Estimated state vector)
% Example 1: [S, xhat] = sr_ukf_state_estimation(y, xhat, Rn, Rv, u, F, S, alpha, beta, L)  
% Author: Daniel Mårtensson, December 2021

% L = Number of states, or sensors in practice.
% beta = used to incorporate prior knowledge of the distribution of x (for Gaussian distributions, beta = 2 is optimal)
% alpha = determines the spread of the sigma points around xhat and alpha is usually set to 0.01 <= alpha <= 1
% S[L * L] = State estimate error covariance
% F(float dx[L], float x[L], float u[L]) = Transition function
% x[L] = State vector
% u[L] = Input signal
% Rv[L * L] = Process noise covariance matrix
% Rn[L * L] = Measurement noise covariance matrix
% xhat[L] = Estimated state (our input)
% y[L] = Measurement state (our output)
function [S, xhat] = sr_ukf_state_estimation(y, xhat, Rn, Rv, u, F, S, alpha, beta, L)  
  % Predict: Create the weights
	kappa = 0; % kappa is 0 for state estimation
	[Wc, Wm] = create_weights(alpha, beta, kappa, L);
  
  % Predict: Create sigma point matrix for F function 
  [X] = create_sigma_point_matrix(xhat, S, alpha, kappa, L);
  
  % Predict: Compute the transition function F
  [Xstar] = compute_transistion_function(X, u, F, L);
  
  % Predict: Multiply sigma points to weights for xhat
	[xhat] = multiply_sigma_point_matrix_to_weights(Xstar, Wm, L);
  
  % Update: Create measurement covariance matrix
	[S] = create_state_estimation_error_covariance_matrix(Wc, Xstar, xhat, Rv, L);
  
  % Predict: Create sigma point matrix for H function. This is the updated version of SR-UKF paper. The old SR-UKF paper don't have this
  [X] = create_sigma_point_matrix(xhat, S, alpha, kappa, L); 
 
  % Predict: Compute the observability function
  [Y] = H(X);
  
  % Predict: Multiply sigma points to weights for yhat
	[yhat] = multiply_sigma_point_matrix_to_weights(Y, Wm, L);
  
  % Update: Create measurement covariance matrix
	[Sy] = create_state_estimation_error_covariance_matrix(Wc, Y, yhat, Rn, L);
  
  % Update: Create parameter covariance matrix
	[Pxy] = create_state_cross_covariance_matrix(Wc, X, Y, xhat, yhat, L);
  
  % Update: Perform parameter update and covariance update
	[S, xhat] = update_state_covarariance_matrix_and_state_estimation_vector(S, xhat, yhat, y, Sy, Pxy, L);
  
end

function [Wc, Wm] = create_weights(alpha, beta, kappa, L)
	% Create the size N
	N = 2 * L + 1;
  
  % Create vectors
  Wc = zeros(1, N);
	Wm = zeros(1, N);

	% Compute lambda and gamma parameters
	lambda = alpha * alpha * (L + kappa) - L;

	% Insert at first index
	Wm(1) = lambda/(L + lambda);
	Wc(1) = Wm(1) + 1 - alpha * alpha + beta;

	% The rest of the indexes are the same 
  Wc(2:N) = 0.5 / (L + lambda);
  Wm(2:N) = Wc(2:N);
end

function [X] = create_sigma_point_matrix(xhat, S, alpha, kappa, L) 
	% Compute lambda and gamma parameters
	lambda = alpha * alpha * (L + kappa) - L;
	gamma = sqrt(L + lambda);

  % Construct sigma point matrix - Notice that (:, 1:L) must be used because S is diagnoal matrix - GNU Octave issue perhaps
  A = xhat + gamma*S(:, 1:L);
  B = xhat - gamma*S(:, 1:L);
  X = [xhat, A, B];
end

function [Xstar] = compute_transistion_function(X, u, F, L)
	% Create the size N
	N = 2 * L + 1;

  % Create the outputs from the transition function
  Xstar = zeros(L, N);
  
  % Call the F transition function with X matrix 
  for i = 1:N
    Xstar(:, i) = F(X(:, i), u);
  end
end

function [xhat] = multiply_sigma_point_matrix_to_weights(X, W, L)
	% Create the size N
	N = 2 * L + 1;

	% Create xhat
  xhat = zeros(L, 1);
  
  % Multiply xhat = W*X
  for i = 1:N
    xhat = xhat + W(i)*X(:, i);
  end
end

function [S] = create_state_estimation_error_covariance_matrix(Wc, X, xhat, R, L)
  % Pick up the second weight
  weight2 = sqrt(abs(Wc(2)));
  
  % Find economy size S by using QR-decomposition
  [~, S] = qr([weight2*(X-xhat), sqrt(R)]', 0);
    
  % Use cholupdate
  if(Wc(1) < 0)
    S = cholupdate(S, X(:, 1) - xhat, '-');
  else
    S = cholupdate(S, X(:, 1) - xhat, '+');
  end
end

function [Y] = H(X)
  Y = X; % Assume that the observability function is just an identity matrix
end

function [Pxy] = create_state_cross_covariance_matrix(Wc, X, Y, xhat, yhat, L)
  % Substract 
  X = X - xhat;  
  Y = Y - yhat;
  
  % Create Pwd
  Pxy = X*diag(Wc)*Y';
end

function [S, xhat] = update_state_covarariance_matrix_and_state_estimation_vector(S, xhat, yhat, y, Sy, Pxy, L)
  % Create kalman gain matrix
  K = Pxy*inv(Sy'*Sy);
  
  % Do parameter estimate update 
  xhat = xhat + K*(y - yhat);
  
  % Compute U
  U = K*Sy;
  
  % Update Sw
  for i = 1:L
    S = cholupdate(S, U(:, i), '-');
  end
end
