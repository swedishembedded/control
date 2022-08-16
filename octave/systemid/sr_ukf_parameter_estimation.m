% Square Root Unscented Kalman Filter For Parameter Estimation (A better version than regular UKF)
% Input: Se list below
% Output: Sw(Parameter estimate error covariance), what(Estimated parameter vector)
% Example 1: [Sw, what] = sr_ukf_parameter_estimation(d, what, Re, x, G, lambda_rls, Sw, alpha, beta, L)
% Author: Daniel Mårtensson, December 2021

% L = Number of states, or sensors in practice.
% beta = used to incorporate prior knowledge of the distribution of x (for Gaussian distributions, beta = 2 is optimal)
% alpha = determines the spread of the sigma points around what and alpha is usually set to 0.01 <= alpha <= 1
% Sw[L * L] = Parameter estimate error covariance
% lambda_rls = Scalar factor 0 <= lambda_rls <= 1. A good number is close to 1 like 0.995
% dw[L] = G(x[L], w[L]) = Transition function with unknown parameters
% x[L] = State vector
% w[L] = Parameter vector
% Re[L * L] = Measurement noise covariance matrix
% what[L] = Estimated parameter (our input)
% d[L] = Measurement parameter (our output)
function [Sw, what] = sr_ukf_parameter_estimation(d, what, Re, x, G, lambda_rls, Sw, alpha, beta, L)  
  % Predict: Create the weights
  kappa = 3 - L; % kappa is 3 - L for parameter estimation
  [Wc, Wm] = create_weights(alpha, beta, kappa, L);
  
  % Predict: Scale Sw with lambda_rls
  [Sw] = scale_Sw_with_lambda_rls_factor(Sw, lambda_rls);
  
  % Predict: Create sigma point matrix for G function
  [W] = create_sigma_point_matrix(what, Sw, alpha, kappa, L);
  
  % Predict: Compute the model G
  [D] = compute_transistion_function(W, x, G, L);
  
  % Predict: Multiply sigma points to weights for dhat
  [dhat] = multiply_sigma_point_matrix_to_weights(D, Wm, L);
  
  % Update: Create measurement covariance matrix
  [Sd] = create_state_estimation_error_covariance_matrix(Wc, D, dhat, Re, L);
  
  % Update: Create parameter covariance matrix
  [Pwd] = create_state_cross_covariance_matrix(Wc, W, D, what, dhat, L);
  
  % Update: Perform parameter update and covariance update
  [Sw, what] = update_state_covarariance_matrix_and_state_estimation_vector(Sw, what, dhat, d, Sd, Pwd, L);
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

function [Sw] = scale_Sw_with_lambda_rls_factor(Sw, lambda_rls)
  % Apply scalar factor to Sw
  Sw = Sw*1.0/sqrt(lambda_rls);
end

function [W] = create_sigma_point_matrix(what, Sw, alpha, kappa, L) 
  % Compute lambda and gamma parameters
  lambda = alpha * alpha * (L + kappa) - L;
  gamma = sqrt(L + lambda);

  % Construct sigma point matrix - Notice that (:, 1:L) must be used because Sw is diagnoal matrix - GNU Octave issue perhaps
  A = what + gamma*Sw(:, 1:L);
  B = what - gamma*Sw(:, 1:L);
  W = [what, A, B];
end

function [D] = compute_transistion_function(W, x, G, L)
  % Create the size N
  N = 2 * L + 1;

  % Create the outputs from the transition function
  D = zeros(L, N);
  
  % Call the G transition function with W matrix 
  for i = 1:N
    D(:, i) = G(x, W(:, i));
  end
end

function [dhat] = multiply_sigma_point_matrix_to_weights(D, Wm, L)
  % Create the size N
  N = 2 * L + 1;

  % Create dhat
  dhat = zeros(L, 1);
  
  % Multiply dhat = Wm*D 
  for i = 1:N
    dhat = dhat + Wm(i)*D(:, i);
  end
end

function [Sd] = create_state_estimation_error_covariance_matrix(Wc, D, dhat, Re, L)
  % Pick up the second weight
  weight2 = sqrt(abs(Wc(2)));
  
  % Find economy sized Sd by using QR-decomposition
  [~, Sd] = qr([weight2*(D-dhat), sqrt(Re)]', 0);
    
  % Use cholupdate
  if(Wc(1) < 0)
    Sd = cholupdate(Sd, D(:, 1) - dhat, '-');
  else
    Sd = cholupdate(Sd, D(:, 1) - dhat, '+');
  end
end

function [Pwd] = create_state_cross_covariance_matrix(Wc, W, D, what, dhat, L)
  % Substract 
  W = W - what;  
  D = D - dhat;
  
  % Create Pwd
  Pwd = W*diag(Wc)*D';
end

function [Sw, what] = update_state_covarariance_matrix_and_state_estimation_vector(Sw, what, dhat, d, Sd, Pwd, L)
  % Create kalman gain matrix
  K = Pwd*inv(Sd'*Sd);
  
  % Do parameter estimate update 
  what = what + K*(d - dhat);
  
  % Compute U
  U = K*Sd;
  
  % Update Sw
  for i = 1:L
    Sw = cholupdate(Sw, U(:, i), '-');
  end
end
