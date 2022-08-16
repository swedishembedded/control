% Independent Component Analysis
% Input: X(Mixed signal matrix)
% Output: S(Estimated signal matrix)
% Example 1: [Y] = ica(X);
% Author: Daniel Mårtensson, November 2021

function [S] = ica(varargin)
 % Check if there is some input arguments
  if(isempty (varargin))
    error ('Missing signal matrix X')
  else
    X = varargin{1};
  end
  
  % Center the data - This will return X transpose
  X = center_data(X);
  
  % Whitening the data
  Z = whitening_data(X);
  
  % Perform fast ICA algorithm
  W = fastica(Z);
  
  % Estimate data 
  S = estimate_data(W, Z);
end

function X = center_data(X)  
  % Do transpose
  X = X';                                  
    
  % Perform centering of X
  X = X - mean(X);
end

function Z = whitening_data(X)
  % Create covaraiance matrix
  Xcov    = cov(X);                           
  
  % Compute eigenvalues D and eigenvectors E
  [E,D]   = eig(Xcov);                        

  % Apply whitening equation
  Z = E*1/sqrt(D)*E'*X';                      
end

function W = fastica(Z)
  % Get sizes
  N = size(Z, 1);
  M = size(Z, 2);
  
  % Create the return back matrix, which is the inverse matrix
  W = 0.5*ones(N,N);                              
  
  % Set fixed iterations
  iterations = 100;                           
  
  % Do the algorithm
  for p = 1:N
    wp = ones(N,1)*0.5;
    wp = wp / sqrt(wp'*wp);
    for i = 1:iterations
        G = tanh(wp'*Z);
        Gder = 1-tanh(wp'*Z).^2;
        wp = 1/M*Z*G' - 1/M*Gder*ones(M,1)*wp;
        dumsum = zeros(N,1);
        for j = 1:p-1
            dumsum = dumsum + wp'*W(:,j)*W(:,j);
        end
        wp = wp - dumsum;        
        wp = wp / sqrt(wp'*wp);
    end
    W(:,p) = wp;
  end
end

function S = estimate_data(W, Z)
  %The factor sqrt(2) is an empirical constant added to make the predictions fit the data properly. 
  W = W/sqrt(2);                              
  
  % Find the estimated data
  S = W'*Z;
end
