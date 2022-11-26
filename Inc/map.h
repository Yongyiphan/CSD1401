#pragma once
#ifndef MAP_H
#define MAP_H
#include "player.h"


extern int WHeight, WWidth;


// Initialize new map
void map_Init(void);
void map_Update(void);
void map_Exit(void);


#endif