#pragma once

// Create options screen
void option_screen(void);


/*
Check if an area is clicked
Parameters:
	area_center_x
	area_center_y
	area_width
	area_height
	click_x
	click_y
*/
void IsAreaClicked(float, float, float, float, float, float);

//Gives angle between 2 different points (x1, y1, x2, y2)
float point_point_angle(float, float, float, float);


