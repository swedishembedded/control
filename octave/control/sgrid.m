% SPDX-License-Identifier: MIT
% Author: Daniel Mårtensson, 2017 Oktober

function sgrid(z, wn)
	% Do the circle
	ang=0:0.01:2*pi;
	xp=wn*cos(ang);
	yp=wn*sin(ang);

	% Do the slope lines
	y1 = [0 wn*sqrt(1-z^2)];
	y2 = [0 -wn*sqrt(1-z^2)];
	x1 = [0 -z*wn];
	x2 = [0 -z*wn];

	% Plot now
	hold on
	plot(xp, yp,'.-', x1, y1, '.-', x2, y2, '.-')
end
