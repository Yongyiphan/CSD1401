#pragma once
#ifndef MAP_H
#define MAP_H


#define Coarse_Spacing 50
#define Fine_Spacing 10
extern int WHeight, WWidth;



// Initialize new map
void map_Init(void);
void map_Update(void);
void map_Exit(void);


#endif