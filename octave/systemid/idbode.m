% SPDX-License-Identifier: MIT
% Plot a bode diagram from frequency data
% Input: u(frequency input), y(frequency response), w(frequencies)
% Example: idbode(u, y, w)
% Author: Daniel Mårtensson, April 2020

function idbode(varargin)
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

	% Get the frequencies
	if(length(varargin) >= 3)
		w = varargin{3};
	else
		error('Missing frequencies');
	end

	% Get the size of u or y and w
	m = size(y, 1);
	n = size(w, 2);
	l = n/2;

	% Do Fast Fourier Transform for every input signal
	for i = 1:m
		% Do FFT
		fy = fft(y(i, 1:n));
		fu = fft(u(i, 1:n));

		% Cut the frequency into half too and multiply it with 4
		W = w(i, 1:l)*4;
		if(W(i, 1) <= 0)
			W(1) = W(2); % Prevent zeros on the first index. In case if you used w = linspace(0,...
		end

		% Create the complex magnitudes and cut it to half
		H = (fy./fu)(1:l);

		% Plot the bode diagram of measurement data
		figure('Name', sprintf(strcat('Transfer function: ', num2str(i))))
		subplot(2,1,1)
		semilogx(W(i, 1:l), 20*log10(abs(H)));
		ylabel('Magnitude [dB]');
		grid on
		subplot(2,1,2)

		BodeAngles = angle(H) * 180/pi;
		semilogx(W(i, 1:l), BodeAngles);
		ylabel('Phase [deg]');
		xlabel('Frequency [rad/s]');
		grid on
	end
end
