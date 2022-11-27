#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include "player.h"

/* --------------------------------------
* File Level Documentation
* @author	Geoffrey Cho Jian Ming
* @email	g.cho@digipen.edu
* @file		Camera.h
* @brief	This file contains all functions required for camera movement
			- Displaces any object placed below the camera function
			- Fixes the player to the middle of the screen
* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/
void CameraDemo_Init(void);
void CameraDemo_Update(Player *P, CP_Matrix *trans);


#endif