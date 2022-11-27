#pragma once
#ifndef MAP_H
#define MAP_H
/*---------------------------------------------------
* File Level Documentation
@contributors	Sen chuan Tay, Michael Mah, Edgar Yong, Geoffrey Cho
@file			map.h
@brief			Main File for Zombie Break Gameplay

* Copyright 2022 Digipen, All Rights Reserved.
*//*-----------------------------------------------*/
#include "player.h"


extern int WHeight, WWidth;


// Initialize new map
void map_Init(void);
void map_Update(void);
void map_Exit(void);


#endif