% SPDX-License-Identifier: MIT
% Past-Input Multivariable Output-Error State Space
% Input: u(input signal), y(output signal), k(Hankel row length), sampleTime, delay(optional)
% Output: sysd(Discrete state space model)
% Example 1: [sysd] = pimoesp(u, y, k, sampleTime, delay);
% Example 2: [sysd] = pimoesp(u, y, k, sampleTime);
% Author: Daniel Mårtensson, December 2017

function [sysd] = pimoesp(varargin)
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
	if(length(varargin) >= 5)
		delay = varargin{5};
	else
		delay = 0; % If no delay was given
	end

	% Check if u and y has the same length
	if(length(u) ~= length(y))
		error('Input(u) and output(y) has not the same length')
	end

	% Get the size of output
	[l,ny] = size(y);
	% Get the size of input
	[m,nu] = size(u);

	% Create the amout of columns
	j = ny-2*k+1;

	% Create hankel matrecies
	U = hank(u, 2*k, j);
	Y = hank(y, 2*k, j);

	% Create the past, future hankel matrecies
	Up = U(1:k*m, :);
	Uf = U((k*m+1):2*k*m, :);

	% For output too.
	Yp = Y(1:k*l,:);
	Yf = Y(k*l+1:2*k*l,:);

	% Get the size of the hankel matrecies
	km = size(Up, 1);
	kl = size(Yp, 1);

	% Create the Wp matrix
	Wp = [Up; Yp];

	% Do QR decomposition
	%[R32, R31, R22, R21, R11] = QR([Uf;Up;Yp;Yf], km, kl);
	[L11, L41, L42, L43] = QR([Uf;Up;Yp;Yf], km, kl);

	% Orthogonal projections
	%AbC = R32*pinv(R22)*Wp;

	% Do singular value decomposition
	[U,S,V] = svd([L42 L43], 'econ');

	% Do model reduction
	[U1, U2, S1, V1, nx] = modelReduction(U, S, V);

	% Create the observability matrix
	OBSV = U1*sqrtm(S1);

	% Find Cd and Ad from observability matrix
	Cd = OBSV(1:l, 1:nx);
	Ad = pinv(OBSV(1:l*(k-1), 1:nx))*OBSV((l+1):l*k, 1:nx);

	% Find D and B matrix
	Z = U2'*L41/L11;

	% Create empty vectors
	XX = [];
	RR = [];

	% Do least square
	for j = 1:k
		XX = [XX; Z(:,m*(j-1)+1:m*j)];
		OBSVj = OBSV(1:l*(k-j),:);
		kj = [zeros(l*(j-1),l) zeros(l*(j-1),nx); eye(l) zeros(l,nx); zeros(l*(k-j),l) OBSVj];
		RR = [RR; U2'*kj];
	end
	% Done - Extract
	DB = pinv(RR)*XX;

	% Find Db and Bd matrix
	Dd = DB(1:l,:);
	Bd = DB(l+1:size(DB,1),:);

	% Create the state space model
	sysd = ss(delay, Ad, Bd, Cd, Dd);
	sysd.sampleTime = sampleTime;

end

function [U1, U2, S1, V1, nx] = modelReduction(U, S, V)
	% Plot singular values
	stem(1:length(S), diag(S));
	title('Hankel Singular values');
	xlabel('Amount of singular values');
	ylabel('Value');

	% Choose system dimension n - Remember that you can use modred.m to reduce some states too!
	nx = inputdlg('Choose the state dimension by looking at hankel singular values: ');
	nx = str2num(cell2mat(nx));

	[um, un] = size(U);
	[sm, sn] = size(S);

	if mod(un, 2) > 0
		error('Try another k - Hankel row length')
	end

	% Split U into U1 and U2
	U1 = U(:, 1:un/2);
	U2 = U(:, (1 + un/2):un);

	% Do the model reduction
	U1 = U1(:, 1:nx);
	U2 = U2(:, 1:nx);
	S1 = S(1:nx, 1:nx);
	V1 = V(:, 1:nx);
end

function [L11, L41, L42, L43] = QR(A, km, kl)
	% QR
	[Q,L] = qr(A',0);

	% Transpose so A = L*Q
	Q=Q';
	L=L';

	% Split
	L11 = L(1:km, 1:km);
	L21 = L(km+1:2*km, 1:km);
	L22 = L(km+1:2*km, km+1:2*km);
	L31 = L(2*km+1:2*km+kl, 1:km);
	L32 = L(2*km+1:2*km+kl, km+1:2*km);
	L33 = L(2*km+1:2*km+kl, 2*km+1:2*km+kl);
	L41 = L(2*km+kl+1:2*km+2*kl, 1:km);
	L42 = L(2*km+kl+1:2*km+2*kl, km+1:2*km);
	L43 = L(2*km+kl+1:2*km+2*kl, 2*km+1:2*km+kl);
	L44 = L(2*km+kl+1:2*km+2*kl, 2*km+kl+1:2*km+2*kl);

	R11 = L11;
	R21 = [L21; L31];
	R22 = [L22 zeros(km,kl); L32 L33];
	R31 = L41;
	R32 = [L42 L43];
end

function [H] = hank(g, i, j) 
	% Get size
	l = size(g, 1);
	% Create a large hankel matrix
	H = zeros(l*i,j);

	for k=1:i
		H((k-1)*l+1:k*l,:) = g(:,k:k+j-1);
	end

end
