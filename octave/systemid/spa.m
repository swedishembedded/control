% Plot bode spectral analysis plot using Fast Fourier Transform
% Input: y(frequency output), t(time)
% Output: amp(amplitude), wout(frequencies)
% Example 1: [amp, wout] = spa(y, t);
% Author: Daniel Mårtensson, November 2017

function [amp, wout] = spa(varargin)
  % Check if there is any input
  if(isempty(varargin))
    error('Missing imputs')
  end

  
  % Get output
  if(length(varargin) >= 1)
    y = varargin{1};
  else
    error('Missing output');
  end
  
  % Get time
  if(length(varargin) >= 2)
    t = varargin{2};
  else
    error('Missing time');
  end
  
  Ts = t(2)-t(1); % Sampling time
  Fs = 1/Ts; % Sampling rate
  % Do FFT
  F = fft(y, Fs);
  Y = abs(F);
  
  % Compute the frequencies 
  freq = (0:Fs-1)(1:end/2 +1);
  % Compute the amplitudes
  Y = Y(1:end/2)/length(Y)*2;
  
  plot(freq, Y)
  ylabel('Amplitude')
  xlabel('Frequency [Hz]')
  
  % Return values
  amp = 20*log10(Y);
  wout = freq;

end
