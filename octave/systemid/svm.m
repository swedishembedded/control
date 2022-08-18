% SPDX-License-Identifier: MIT
% Support Vector Machine with C code generation
% Input: X(data in x-axis), Y(data in y-axis)
% Output: X_point(coordinates in x-axis), Y_point(coordinates in y-axis), amount_of_supports_for_class(how many points for each class)
% Example 1: [X_point, Y_point, amount_of_supports_for_class] = svm(X, Y);
% Author: Daniel Mårtensson, December 2021

function [X_point, Y_point, amount_of_supports_for_class] = svm(X, Y)
	% First print our data
	[legend_labels] = print_data(X, Y);

	% Then place out our support points
	[X_point, Y_point, amount_of_supports_for_class] = place_out_supports_points(X, Y, legend_labels);

	% Check if our support points works
	check_points_support(X, Y, X_point, Y_point);

	% Ask the user what the file name should have
	[file_name] = ask_user_about_file_name();

	% Generate C code
	[c_source, c_header, X_point, Y_point, amount_of_supports_for_class] = generate_c_code(X_point, Y_point, amount_of_supports_for_class, file_name);

	% Save the C code into a file
	save_c_code_into_a_file(c_source, c_header, file_name);
end

function [legend_labels] = print_data(X, Y)
	% Get the amount of classes
	c = size(X, 1);

	% Plot the data classes 
	legend_labels = {};
	for i = 1:c
		scatter(X(i, :), Y(i, :));
		hold on
		legend_labels{i} = sprintf('Data %i', i);
	end
	legend(legend_labels);
	grid on
end

function [X_point, Y_point, amount_of_supports_for_class] = place_out_supports_points(X, Y, legend_labels)
	% Get the amount of classes
	c = size(X, 1);

	% Start set out the supports points
	X_point = zeros(c, 256);
	Y_point = zeros(c, 256);
	amount_of_supports_for_class = zeros(1, c);
	for i = 1:c
		% buttonnumber == 1 indicates left click
		% buttonnumber == 3 indicates right click
		buttonnumber = 1;
		j = 1;
		title(sprintf('Place SVM points for Data %i', i), 'FontSize', 30);
		xlabel('Left click to set out a point. Right click to go to next data class');
		while buttonnumber == 1
			% Left-click on the scatter plot
			[x, y, buttonnumber] = ginput(1);

			% Abort to next data set
			if(buttonnumber == 3)
				break;
			end

			% Place the x and y -
			X_point(i, j) = x;
			Y_point(i, j) = y;

			% Remember how many points we have set ut for each data class
			amount_of_supports_for_class(i) = amount_of_supports_for_class(i) + 1;

			% Plot the dots as a line
			X_line = X_point(i, 1:j);
			Y_line = Y_point(i, 1:j);
			if(length(X_line) == 1)
				scatter(X_line, Y_line, '.')
			else
				plot(X_line, Y_line, '-k')
			end

			% Place out the legends so we don't get new legends
			legend(legend_labels)

			% Increment
			j = j + 1;
		end

		% The last line will close the gap between the end and the beginning
		X_point(i, j) = X_point(i, 1);
		Y_point(i, j) = Y_point(i, 1);

		% Remember how many points we have set ut for each data class
		amount_of_supports_for_class(i) = amount_of_supports_for_class(i) + 1;

		% Plot the points as a line 
		X_line = X_point(i, 1:j);
		Y_line = Y_point(i, 1:j);
		plot(X_line, Y_line, '-k')

		% Place out the legends so we don't get new legends
		legend(legend_labels)
	end
end

function check_points_support(X, Y, X_point, Y_point)
	% Get the amount of classes
	c = size(X_point, 1);

	% Do a quick check if the SVM points are correctly placed
	for i = 1:c
		% Get the points
		all_X_points = X_point(i, :);
		all_Y_points = Y_point(i, :);

		% Begin to count
		counted_points = sum(inpolygon(X(i,:) , Y(i, :), all_X_points, all_Y_points));
		if(counted_points == length(X(i, :)))
			disp(sprintf('Checking for class %i is OK - Good generated SVM supports', i));
		else
			error(sprintf('Checking for class %i failed beacause the generated SVM points excluding data', i));
		end
	end
end

function [file_name] = ask_user_about_file_name()
	file_name = cell2mat(inputdlg('Enter a file name without file extension', 'Support Vector Machine C code generation'));
	if(length(file_name) == 0)
		error('You need to enter a file name!');
	end
end

function [c_source, c_header, X_point, Y_point, amount_of_supports_for_class] = generate_c_code(X_point, Y_point, amount_of_supports_for_class, file_name)
	% Find maximum column for X_point_str and Y_point_str
	len_px_py = max(amount_of_supports_for_class);

	% Create svm_classes
	svm_classes = size(X_point, 1);

	% Cut the matrix so we can avoid all zeros
	X_point = X_point(:, 1:len_px_py);
	Y_point = Y_point(:, 1:len_px_py);

	% Create px matrix with 4 decimals - Notice that X_point must be transpose
	px = sprintf('%0.4ff,', X_point');
	px(end) = ''; % Remove the last ','

	% Create py matrix with 4 decimals - Notice that Y_point must be transpose
	py = sprintf('%0.4ff,', Y_point');
	py(end) = ''; % Remove the last ','

	% Create p array
	p = sprintf('%i,', amount_of_supports_for_class);
	p(end) = ''; % Remove the last ','

	% Write this c source into a file
	c_source = {sprintf('#include "%s.h"', file_name);
	'/*'
	' * Support vector machine for classification using CControl library'
	' * x[m] = Data in x-axis'
	' * y[m] = Data in y-axis'
	' * point_counter_list[svm_classes] = Array that holds how many points each class got from the data'
	' *'
	' * This function can hold 256 classes. Each index in the array point_counter_list got a positive number'
	' * Largest number in the array point_counter_list can reveal what class the data belongs to by looking at the index of the array'
	' */'
	sprintf('#define svm_classes %i				/* How many classes this this SVM function handle */', svm_classes);
	sprintf('#define len_px_py %i					/* Length of the px and py matrix */', len_px_py);
	''
	sprintf('void %s(float x[], float y[], uint16_t m, uint16_t point_counter_list[]){', file_name);
	''
	'	/* Create the polygon coordinates */'
	sprintf('	float px[svm_classes*len_px_py] = {%s};', px);
	sprintf('	float py[svm_classes*len_px_py] = {%s};', py);
	''
	'	/* How much data in each line of the polygon coordinates */'
	sprintf('	uint8_t p[svm_classes] = {%s};', p);
	''
	'	/* Count how many points the data gives for each class */'
	'	memset(point_counter_list, 0, svm_classes*sizeof(uint16_t));'
	'	for(uint8_t i = 0; i < svm_classes; i++)'
	'		for(uint16_t j = 0; j < m; j++)'
	'			point_counter_list[i] += inpolygon(x[j], y[j], &px[i*len_px_py], &py[i*len_px_py], p[i]);'
	'}'};

	% Create the header file
	c_header = {sprintf('#ifndef %s', strcat(upper(file_name), '_H_'));
	sprintf('#define %s', strcat(upper(file_name), '_H_'))
	''
	'#include "CControl/Headers/Functions.h" /* You need to point this location to the CControl header file */'
	''
	'#ifdef __cplusplus'
	'extern "C" {'
	'#endif'
	''
	sprintf('void %s(float x[], float y[], uint16_t m, uint16_t point_counter_list[]);', file_name);
	''
	'#ifdef __cplusplus'
	'}'
	'#endif'
	''
	'#endif'};

end

function save_c_code_into_a_file(c_source, c_header, file_name)
	% Create the source file
	fid = fopen(strcat(file_name, '.c'), 'wt');
	for i = 1:size(c_source, 1)
		fprintf(fid, "%s\n", cell2mat(c_source(i)));
	end
	fclose(fid);

	% Create the header file
	fid = fopen(strcat(file_name, '.h'), 'wt');
	for i = 1:size(c_header, 1)
		fprintf(fid, "%s\n", cell2mat(c_header(i)));
	end
	fclose(fid);

	% Tell the users about the fantastic news
	disp(sprintf('The source file %s.c and header file %s.h has been generated at the location %s', file_name, file_name, pwd));
end
