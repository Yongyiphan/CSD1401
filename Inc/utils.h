#pragma once

// Create options screen
void option_screen(int*);


/*
Check if an area is clicked
Parameters:	area_center_x, area_center_y, area_width, area_height, click_x, click_y
*/
int IsAreaClicked(float, float, float, float, float, float);