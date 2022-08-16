% Observer Controller Identification
% Input: r(reference signal), uf(feedback signal), y(output signal), sampleTime, delay(optional), regularization(optional), systemorder(optional)
% Output: sysd(Discrete state space model), K(Kalman gain matrix)
% Example 1: [sysd, K, L] = ocid(r, uf, y, sampleTime, regularization, systemorder);
% Example 2: [sysd, K, L] = ocid(r, uf, y, sampleTime);
% Author: Daniel Mårtensson, 24 April 2020. Follows NASA document ID 19920072673

function [sysd, K, L] = ocid(varargin)
  % Check if there is any input
  if(isempty(varargin))
    error('Missing imputs')
  end
  
  % Get reference
  if(length(varargin) >= 1)
    r = varargin{1};
  else
    error('Missing reference')
  end
  
  % Get feedback
  if(length(varargin) >= 2)
    uf = varargin{2};
  else
    error('Missing feedback')
  end
  
  % Get output
  if(length(varargin) >= 3)
    y = varargin{3};
  else
    error('Missing output')
  end
  
  % Get the sample time
  if(length(varargin) >= 4)
    sampleTime = varargin{4};
  else
    error('Missing sample time');
  end
  
  % Get the regularization parameter
  if(length(varargin) >= 5)
    regularization = varargin{5};
    if (regularization <= 0)
      regularization = 0;
    end
  else
    regularization = 0; % If no regularization was given
  end
  
  % Get the order if the system
  if(length(varargin) >= 6)
    systemorder = varargin{6};
    if (systemorder <= 0)
      systemorder = -1;
    end
  else
    systemorder = -1; % If no order was given
  end
  
  % Check if u and y has the same length
  if(or(length(uf) ~= length(y), length(uf) ~= length(r)))
    error('Feedback(uf) and output(y) and reference(r) has not the same length')
  end
  
  % Check if y and uf and r can be diveded with 2
  if mod(length(uf), 2) > 0
    uf = uf(:, 1:end-1);
    y = y(:, 1:end-1);
    r = r(:, 1:end-1);
  end
  
  % Get the dimensions first
  q = size(y, 1); % Dimension of output
  l = size(y, 2); % Total length
  m = size(uf, 1); % Dimension of input
  p = l/2-1; % We select half minus -1 else Ybar can be unstable for SISO noise free case
  
  % Save the system markov parameters and observer markov parameters here
  D = zeros(q, m);
  Y = zeros(q+m, p+p);
  Yrow = 1; % Rows for Y
  
  % This method computes markov parameters for every signal each. The reason where it's 1 everywhere is because we
  % Assume that m = 1 and q = 1 for every signal. So q+m = 1+1 for every signal each.
  for j = 1:q
    % Create V matrix
    V = zeros((1+1)*p + 1, l-p);
    
    % Begin with the first row for V
    V(1, 1:l-p) = -uf(j, p+1:l) + r(j, p+1:l);
    
    % Now do the rest (1+1)*p rows for V. We want to implement v = [u;y] into V - This is equation 8
    positionrow = 1;
    for row = 1:(1+1):(1+1)*p
      % For u
      V(row + 1, 1:l-p) = -uf(j, p-positionrow+1:l-positionrow) + r(j, p-positionrow+1:l-positionrow);
      % For y
      V(row + 1 + 1, 1:l-p) = y(j, p-positionrow+1:l-positionrow);
      positionrow = positionrow + 1;
    end
    
    % Important to have part of y and uf
    yt = [y(j, p+1:l); uf(j, p+1:l)];
    
    % Solve for non-filtred markov parameters with tikhonov regularization
    Ybar = yt*inv(V'*V + regularization*eye(size(V'*V)))*V';
    
    % Get D matrix
    D0 = Ybar(1, 1); % This is YBar_{0}
    D(j, j) = D0; % Save
    
    % Remove D from Ybar
    YbarNoD = zeros(1+1, (1+1)*p);
    YbarNoD(1:1+1, 1:(1+1)*p) = Ybar(1:1+1, 1+1:(1+1)*p+1);
    
    % We need to split YbarNoD into Ybar11, Ybar12, Ybar21 and Ybar22
    Ybar11 = YbarNoD(1:1, 1:1+1:(1+1)*p); 
    Ybar12 = YbarNoD(1:1, (1+1):1+1:(1+1)*p);
    Ybar21 = YbarNoD(1+1:1+1, 1:1+1:(1+1)*p);
    Ybar22 = YbarNoD(1+1:1+1, (1+1):1+1:(1+1)*p);
    
    % Get the markov parameters CA^kB, CA^kG, FA^kB, FA^kG
    [Y11, Y12] = getMarkovCAB_CAG(Ybar11, Ybar12, D0, p);
    [Y21, Y22] = getMarkovFAB_FAG(Ybar21, Ybar22, Y11, Y12, D0, p);
    
    % Save them into one markov parameter Y
    index = 1;
    for i = 1:2:2*p
      Y(Yrow:Yrow+1, i:i+1) = [Y11(index) Y12(index); Y21(index) Y22(index)];
      index = index + 1;
    end
    Yrow = Yrow + q;
  end
  
  % Time to find A, B, C, D, K, L 
  delay = 0;
  [sysd, K, L] = eradcocid(D, Y, sampleTime, delay, systemorder);

end

% Special ERA/DC for just OCID command 
% This is equation 28 in OCID.pdf file. Read also ERADC.pdf file as well
function [sysd, K, L] = eradcocid(D, g, sampleTime, delay, systemorder)
  % Get the number of input
  nu = size(g, 1); 
  
  % Change g for MIMO to diagonal case
  if(nu > 2) % 2 because we have the FAB and FAG markov parameters as well. If nu = 2 = SISO case
    l = length(g);
    G = zeros(nu, l*nu/2); % Divide number of inputs(nu) with 2 because 4 rows is 2*2 input/output, 2 rows is 1*1 input/output
    rowcount = 1;
    beginning = 1;
    for i = 1:nu
      Gcolumn = beginning; % Where we should start. 1 to begin with
      columncount = 1;
      for j = 1:l
        
        % Insert data into Y and count
        G(i, Gcolumn) = g(i, j);
        Gcolumn = Gcolumn + 1;
        columncount = columncount + 1;
        
        % When we av inserted [CAB CAG] then jump two steps to right
        if(columncount > 2)
          columncount = 1;
          Gcolumn = Gcolumn + 2;
        end
        
      end
      
      % This counter is made for the shifting so G will be diagonal
      rowcount = rowcount + 1;
      if(rowcount > 2)
        rowcount = 1;
        beginning = beginning + 2;
      end
      
    end
    g = G; 
  end
  
  % Create hankel matrices 
  H0 = hank(g, 0); % We don't have D inside g here! There fore we start at k = 0
  H1 = hank(g, 1);
  
  % Do data correlations
  R0 = H0*H0';
  R1 = H1*H0';
  
  % Do SVD on R0
  [U,S,V] = svd(R0, 'econ');
  
  % Do model reduction
  [Un, En, Vn, nx] = modelReduction(U, S, V, systemorder);
  
  % Create the A matrix
  Ad = En^(-1/2)*Un'*R1*Vn*En^(-1/2);
  
  % The reason why we are using 1:2:nu and 2:2:nu here is because how
  %   Y = [CAB CAG; 
  %       FAB FAG];
  %   Is shaped. Try to understand how I have placed the data in Y.
  %   Y(Yrow:Yrow+1, i:i+1) = [Y11(index) Y12(index); Y21(index) Y22(index)];
  %  
  %   Example for the impulse response
  %         1    2      3    4     5   6   nu
  %  1 g = [CAB1 CAG1   0    0     0   0
  %  2      FAB1 FAG1   0    0     0   0
  %  3       0     0   CAB2 CAG2   0   0
  %  4       0     0   FAB2 FAG2   0   0
  %  5       0     0    0    0   CAB3 CAG3; 
  %  6       0     0    0    0   FAB3 FAG3];
  %  nu
  
  % From X we can get Bd and K(Kalman gain G)
  Pa = Un*En^(1/2);
  X = pinv(Pa)*H0; 
  Bd = X(1:nx, 1:2:nu);
  K = X(1:nx, 2:2:nu);
  
  % From Pa we can find Cd and L(Control law F)
  Cd = Pa(1:2:nu, 1:nx);
  L = Pa(2:2:nu, 1:nx);
  
  % D matrix
  Dd = D;

  % Create state space model now
  sysd = ss(delay, Ad, Bd, Cd, Dd);
  sysd.sampleTime = sampleTime;
  
end

function [U1, S1, V1, nx] = modelReduction(U, S, V, systemorder)
  % Plot singular values 
  stem(1:length(S), diag(S));
  title('Hankel Singular values');
  xlabel('Amount of singular values');
  ylabel('Value');
  
  if(systemorder == -1)
    % Choose system dimension n - Remember that you can use modred.m to reduce some states too!
    nx = inputdlg('Choose the state dimension by looking at hankel singular values: ');
    nx = str2num(cell2mat(nx));
  else
    nx = systemorder;
  end
  
  % Choose the dimension nx
  U1 = U(:, 1:nx);
  S1 = S(1:nx, 1:nx);
  V1 = V(:, 1:nx);
end

% Create the half square hankel matrix
function [H] = hank(g, k)
  % We got markov parameters g = [g0 g1 g2 g2 g3 ... gl]; with size m*m. g0 = D
  m = size(g, 1);
  n = size(g, 2);
  l = length(g)/(m*2);
  H = zeros(l*m, l*m);
  for i = 1:l
    if(and(i == l, k == 2))
      % This is a special case when g is out of index, just add zeros instead!
      row = g(:, 1 + (k+i-1)*m:(k+i-2)*m + l*m);
      H(1 + (i-1)*m:(i-1)*m + m, 1:l*m) = [row zeros(m, m)]; 
   else
      row = g(:, 1 + (k+i-1)*m:(k+i-1)*m + l*m);
      H(1 + (i-1)*m:(i-1)*m + m, 1:l*m) = row;
    end
  end
end


% This function will find markov parameters Y11 and Y12 from Ybar11 and Ybar12
% In this example, we indexing from 1, not 0.
function [Y11, Y12] = getMarkovCAB_CAG(Ybar11, Ybar12, D, p)
  Y11 = zeros(1, p);
  Y12 = zeros(1, p);
  for k = 1:p
    
    % Find the Ybar11 and Ybar12
    Ybar11_k = Ybar11(k);
    Ybar12_k = Ybar12(k);
    
    % Sum
    Y11sum = 0;
    Y12sum = 0;
    for i = 1:k
      if(k-i == 0)
        Y11sum = Y11sum + Ybar12(i)*D; % Y11(0) = D
      else
        Y11sum = Y11sum + Ybar12(i)*Y11(k-i);
      end
    end
    for i = 1:k-1
      Y12sum = Y12sum + Ybar12(i)*Y12(k-i);
    end
    
    % Save for next iteration
    Y11(k) = Ybar11_k - Y11sum; % CA^kB
    Y12(k) = Ybar12_k - Y12sum; % CA^kG
  end
end

% This function will find markov parameters Y21 and Y22 from Ybar11 and Ybar12
% In this example, we indexing from 1, not 0.
function [Y21, Y22] = getMarkovFAB_FAG(Ybar21, Ybar22, Y11, Y12, D, p)
  Y21 = zeros(1, p);
  Y22 = zeros(1, p);
  for k = 1:p
    
    % Find the Ybar21 and Ybar22
    Ybar21_k = Ybar21(k);
    Ybar22_k = Ybar22(k);
    
    % Sum
    Y21sum = 0;
    Y22sum = 0;
    for i = 1:k
      if(k-i == 0)
        Y21sum = Y21sum + Ybar22(i)*D; % Y11(0) = D
      else
        Y21sum = Y21sum + Ybar22(i)*Y11(k-i);
      end
    end
    for i = 1:k-1
      Y22sum = Y22sum + Ybar22(i)*Y12(k-i);
    end
    
    % Save for next iteration
    Y21(k) = Ybar21_k - Y21sum; % FA^kB
    Y22(k) = Ybar22_k - Y22sum; % FA^kG
  end
end
