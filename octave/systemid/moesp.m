% Multivariable Output-Error State Space
% Input: u(input signal), y(output signal), k(Hankel row length), sampleTime, delay(optional)
% Output: sysd(Discrete state space model)
% Example 1: [sysd] = moesp(u, y, k, sampleTime, delay);
% Example 2: [sysd] = moesp(u, y, k, sampleTime);
% Author: Daniel Mårtensson, December 2017

function [sysd] = moesp(varargin)
  % Check if there is any input
  if(isempty(varargin))
    error('Missing imputs')
  end
  
  % Get input 
  if(length(varargin) >= 1)
    u = varargin{1};
  else
    error('Missing input')
  end
  
  % Get output 
  if(length(varargin) >= 2)
    y = varargin{2};
  else
    error('Missing output')
  end
  
  % Get k - block rows
  if(length(varargin) >= 3)
    k = varargin{3};
  else
    error('Missing amout of block rows')
  end
  
  % Get the sample time
  if(length(varargin) >= 4)
    sampleTime = varargin{4};
  else
    error('Missing sample time');
  end
  
  % Get the delay
  if(length(varargin) >= 4)
    delay = varargin{4};
  else
    delay = 0; % If no delay was given
  end
  
  % Check if u and y has the same length
  if(length(u) ~= length(y))
    error('Input(u) and output(y) has not the same length')
  end
  
  % Get number of inputs (m) and outputs (p)
  m = size(u, 1); 
  p = size(y, 1);
  
  % We got N from k*m + k*p
  N = k*m + k*p;
  
  % Check hos large k number we can have
  if(N + k > length(u))
    error('Sorry, k is to large')
  end
  
  % Get hankel matrices
  Uh = hank(u, k, N);
  Yh = hank(y, k, N);
  
  % Do LQ decomposition
  [L11, L21, L22] = lq(Uh, Yh, k, m, p);
  
  % Do Singular value decomposition
  [U,S,V] = svd(L22);
  
  % Do model reduction
  [U1, U2, S1, V1, n] = modelReduction(U, S, V, k, m, p);
  
  % Create observability matrix 
  OBSV = U1*sqrtm(S1);
  
  % Matrices Ad and Cd
  Cd = OBSV(1:p, 1:n);
  Ad = pinv(OBSV(1:p*(k-1), 1:n))*OBSV((p+1):k*p, 1:n);
  
  %% Matrices Bd and Dd
  Z = U2'*L21/L11;
  
  % Create empty vectors
  XX = []; 
  RR = [];
  
  % Do least square
  for j = 1:k
    XX = [XX; Z(:,m*(j-1)+1:m*j)];
    OBSVj = OBSV(1:p*(k-j),:);
    kj = [zeros(p*(j-1),p) zeros(p*(j-1),n); eye(p) zeros(p,n); zeros(p*(k-j),p) OBSVj];
    RR = [RR; U2'*kj];
  end
  % Done - Extract
  DB = pinv(RR)*XX;
  
  % Find Db and Bd matrix
  Dd = DB(1:p,:);
  Bd = DB(p+1:size(DB,1),:);
  
  % Create the state space model
  sysd = ss(delay, Ad, Bd, Cd, Dd); 
  sysd.sampleTime = sampleTime;
  
end

function [L11, L21, L22] = lq(U, Y, k, m, p)
  [Q, L] = qr([U;Y]');
  
  % Rechange them all!
  Q = Q';
  L = L';
  
  km = k*m;
  kp = k*p;

  % Find the matrices 
  L11 = L(1:km,1:km);
  L21 = L(km+1:km+kp,1:km);
  L22 = L(km+1:km+kp,km+1:km+kp);
  
end


function [U1, U2, S1, V1, n] = modelReduction(U, S, V, k, m, p)
  % Plot singular values 
  stem(diag(S));
  title('Hankel Singular values');
  xlabel('Amount of singular values');
  ylabel('Value');
  
  % Choose system dimension n - Remember that you can use modred.m to reduce some states too!
  n = inputdlg('Choose the state dimension by looking at hankel singular values: ');
  n = str2num(cell2mat(n));
  
  km = k*m;
  kp = k*p;
  U1 = U(1:kp, 1:n);
  U2 = U(1:kp, (1+n):(kp-n));
  S1 = S(1:n, 1:n);
  V1 = V(:, 1:n);
end


function [H] = hank(g, k, N)
  % Create hankel matrix
  H = cell(k, N); 

  for i = 1:k
    for j = 1:N
      H{i,j} = g(:,1+i+j-2);
    end
  end
  
  % Cell to matrix
  H = cell2mat(H);
end