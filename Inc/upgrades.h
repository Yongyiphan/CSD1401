
#pragma once
#include <stdio.h>

#ifndef UPGRADES_H
#define UPGRADES_H
/* --------------------------------------
* File Level Documentation
* @author		Mah Ming Qian Michael
* @email		m.mah@digipen.edu
* @file			upgrades.h
* @brief		Upgrades header file.
					Manages initialization, updating and drawing of the upgrades screen.
					Saves, loads and updates player uprgades.
					Provides an interface to level up or reset the specific 
					upgrades and apply them to the player.
					Utilises a currency to allow players to upgrade as well.

* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/

#define NUM_UPGRADES 7
void upgrades_Init(void);
void upgrades_Update(void);
void upgrades_Exit(void);


typedef struct Upgrades
{
	const char* name;
	int level;
	int cost;
	float stat;
} upgrade;

typedef struct currency
{
	int amount;
} coins;

extern upgrade upgrades[NUM_UPGRADES];
extern coins money;
char* convert_int_to_string(char buffer[], int value);

FILE* openfile(const char* path, const char* mode);
void closefile(FILE* file);

void save_all_upgrades_to_file(void);
void upgrades_read_from_file(void);
void reset_all_upgrades(void);
#endif