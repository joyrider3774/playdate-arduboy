/*
   Copyright (C) 2016 Ignacio Vina (@igvina)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once
#include <Arduboy2.h>
#include <ArduboyPlaytune.h>
#include "Customlib.h"

#define MODE_LOGO         0
#define MODE_MENU         1
#define MODE_GAME         2
#define MODE_CREDITS      3
#define MODE_HELP         4

unsigned int mode;


#define GAME_STOP        0
#define GAME_RUNNING        1

unsigned char game;


#define FPS_60          1
#define FPS_30          2


boolean sound;
boolean immortal;
boolean autosave;
boolean showFPS;
boolean levelLoadedFromEEPROM;

// arduboy.setFrameRate(60 / frameDrop);
unsigned int frameDrop;

#ifdef ARDUBOY_2
Arduboy2 arduboy;
ArduboyPlaytune *tunes;
#else
Arduboy arduboy;
#endif
Customlib helper (arduboy);

//TODO move
const int BALL_STOPS_X = 92;
