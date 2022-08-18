% SPDX-License-Identifier: MIT
% Generates a transfer function from numerator vector and denomerator vector
% Input: numerator, denomerator, delay(optional)
% Example 1: G = tf(num, den, delay)
% Example 2: G = tf(num, den)
% Author: Daniel Mårtensson, 2017 September
% Update: Added padé approximation 2020-06-29

function [G] = tf(varargin)
	% Check if there is some input arguments
	if(isempty (varargin))
		error ('Missing arguments')
	end
	% OK. We have arguments!

	% Get numerator
	if(length(varargin) < 1)
		error('Missing numerator')
	else
		numerator = varargin{1};
	end
	% Get denomerator
	if(length(varargin) < 2)
		error('Missing denomerator')
	else
		denomerator = varargin{2};
	end

	% Check if there is some numerators
	if(isempty (numerator))
		error('Missing numerator');
	end
	% OK. We have numerators!

	% Check if there is some denomerators
	if(isempty (denomerator))
		error('Missing denomerator');
	end
	% OK. We have denomerator!

	% Remove the first 0's in the numerator and denomerator
	% Begin with the numerator
	for i = 1:length(numerator)
		% Find the first value which is larger that 0 or less that 0
		if(or(numerator(i) > 0, numerator(i) < 0))
			% Found it!
			numerator = numerator(1,i:length(numerator));
			break;
		end
	end

	% Then with the denomerator
	for i = 1:length(denomerator)
		% Find the first value which is larger that 0 or less that 0
		if(or(denomerator(i) > 0, denomerator(i) < 0))
			% Found it!
			denomerator = denomerator(1,i:length(denomerator));
			break;
		end
	end

	% Build the transfer function
	numeratorString = stringPoly(numerator); % Get the numerator string
	denomeratorString = stringPoly(denomerator); % Get the denomerator string
	dash = getDashedLine(numeratorString, denomeratorString); % Get the dashed line

	% Save the vale
	G.num = numerator;
	G.den = denomerator;
	% Delaytime
	if(length(varargin) >= 3)
		% Get dashed line delay
		delay = varargin{3};
		if(delay > 0)
			dash = strcat(dash, 'e^(', num2str(-delay), 's', ')');
			% Remove 1 to '' if delay = 1
			if delay == 1
				dash = regexprep(dash,'[1]','');
			end
			G.delay = delay;
		else
			G.delay = 0;
		end
	else
		G.delay = 0;
	end
	% Save
	G.tfnum = numeratorString;
	G.tfdash = dash;
	G.tfden = denomeratorString;
	G.type = 'TF';
	G.sampleTime = 0;

	% Padé approximation
	if(G.delay > 0)
		G = pade(G, 4);
	end
end

% Get a dashed line with dynamical length depending on denomeratorString or numeratorString
function [dash] = getDashedLine(numeratorString, denomeratorString)
	dash = '';
	if(length(numeratorString) <= length(denomeratorString))
		for i = 1:length(denomeratorString)
			dash = strcat(dash,'-');
		end
	else
		for i = 1:length(numeratorString)
			dash = strcat(dash,'-');
		end
	end
end

function [returnString] = stringPoly(array)
	polyString = {}; % Begin with empty cell string

	% Get all to a string cell
	for i = 1:length(array)
		if(and(array(i) ~= 0, length(array) - i ~= 0))
			if(length(array) - i > 1)
				if(and(array(i) <= 1.000001, array(i) >= 0.999999))
					polyString(1, i) = {strcat('s^', num2str(length(array) - i))};
				elseif(and(array(i) >= -1.000001, array(i) <= -0.999999))
					polyString(1, i) = {strcat('-s^', num2str(length(array) - i))};
				else
					polyString(1, i) = {strcat(num2str(array(i)), 's^', num2str(length(array) - i))};
				end
			else
				if(and(array(i) <= 1.000001, array(i) >= 0.999999))
					polyString(1, i) = {strcat('s')};
				elseif(and(array(i) >= -1.000001, array(i) <= -0.999999))
					polyString(1, i) = {strcat('-s')};
				else
					polyString(1, i) = {strcat(num2str(array(i)), 's')};
				end
			end
		else
			polyString(1, i) = {strcat(num2str(array(i)))};
		end
	end

	% Transform that string cell to a complete string
	returnString = '';
	for i = 1:length(polyString)
		% No only-zeros are allowed
		if(and(~strcmp(char(polyString(1,i)), '0'), ~strcmp(char(polyString(1,i)), '-0')))
			if(length(strfind(char(polyString(1,i)), '-')) > 0)
				returnString = [returnString,' ', char(polyString(1,i))];
			else
				if(length(returnString) == 0)
					returnString = [returnString,' ', char(polyString(1,i))];
				else
					returnString = [returnString,' + ', char(polyString(1,i))];
				end
			end
		end
	end
end
