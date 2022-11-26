#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "player.h"

// Create options screen
void option_screen(int*);


/*
Check if an area is clicked
Parameters:	area_center_x, area_center_y, area_width, area_height, click_x, click_y
*/

//Gives angle between 2 different points (x1, y1, x2, y2)
float point_point_angle(float, float, float, float);


int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

// Reinitialize the timer with init
float timer(int isPaused, int init);

int Timer_CountDown(void);

void Draw_Time(float totalElapsedTime);

#endif

