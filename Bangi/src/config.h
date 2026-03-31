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

//#define BETA
#define LOW_MEMORY
#define ARDUBOY_2
//#define SCREEN_CAST
//#define GOD_MODE
//#define PERFORMANCE_TEST

#include <Arduboy2.h>

const char PROGMEM STRING_BY_IGVINA[] = "by @igvina";
const char PROGMEM STRING_GAME_VERSION[] = "v1.0";

#define GAME_ID      31
#define GAME_VERSION 3

#define CUSTOM_EEPROM_START EEPROM_STORAGE_SPACE_START + 128

#define EEPROM_GAME_IDU     CUSTOM_EEPROM_START + 0
#define EEPROM_LAST_LEVEL   CUSTOM_EEPROM_START + 1
#define EEPROM_LAST_LIVES   CUSTOM_EEPROM_START + 2
#define EEPROM_DIFFICULTY   CUSTOM_EEPROM_START + 3
#define EEPROM_SOUND_ON     CUSTOM_EEPROM_START + 4
#define EEPROM_IMMORTAL     CUSTOM_EEPROM_START + 5
#define EEPROM_AUTOSAVE     CUSTOM_EEPROM_START + 6
#define EEPROM_FPS          CUSTOM_EEPROM_START + 7
#define EEPROM_SHOW_FPS     CUSTOM_EEPROM_START + 8

const unsigned char MAX_BALLS = 32;
const unsigned char MAX_RAYS = 2;

const unsigned int INITIAL_LIVES = 5;


// Random gift: 1/LUCKY_GIFTS_RATIO  Ex. 5 -> ( ~ 20%)
//const unsigned int LUCKY_GIFTS_RATIO = 12;
const unsigned int LUCKY_GIFTS_RATIO = 12;


// To allow precise movement (less that 1 pixel per frame), ex. SCREEN_MAGNIFICATION value is 10 -> Virtual screen size = (128, 64) * 10 = 1280 * 640
// Check performance and avoid OVERFLOW

// BETWEEN 10 - 100 should work ok, bigger number for slower accel speeds
//const int SCREEN_MAGNIFICATION = 20;
const int SCREEN_MAGNIFICATION = 80;
const int SCREEN_MAGNIFICATION_20 = SCREEN_MAGNIFICATION / 20;

// NOTE: we get near 60 fps
// ex. 1.5 real pixels per frame = (15 * SCREEN_MAGNIFICATION) / 10;
const unsigned int PLAYER_SPEED = 30 * SCREEN_MAGNIFICATION_20;
const unsigned int RAY_SPEED = 24 * SCREEN_MAGNIFICATION_20;
const unsigned int GIFT_SPEED = 16 * SCREEN_MAGNIFICATION_20;


//BALLS
// X speed uniform, Y speed accelerated by G
// Tested values

const unsigned int BALL_SMALL_SPEED_X = 5 * SCREEN_MAGNIFICATION_20;
const unsigned int BALL_SMALL_SPEED_Y = 24 * SCREEN_MAGNIFICATION_20;

const unsigned int BALL_MEDIUM_SPEED_X = 6 * SCREEN_MAGNIFICATION_20;
const unsigned int BALL_MEDIUM_SPEED_Y = 28 * SCREEN_MAGNIFICATION_20;

const unsigned int BALL_BIG_SPEED_X = 8 * SCREEN_MAGNIFICATION_20;
const unsigned int BALL_BIG_SPEED_Y = 28 * SCREEN_MAGNIFICATION_20;

const unsigned int GRAVITY_ACCEL = 1 * SCREEN_MAGNIFICATION_20 / 2;
