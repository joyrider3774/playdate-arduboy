#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduboy2Common.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_MENU_HELP              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_SOUNDFX           5

//define game states (on main menu)
#define STATE_GAME_NEXT_STAGE        6
#define STATE_GAME_PLAYING           7
#define STATE_GAME_PAUSE             8
#define STATE_GAME_OVER              9
#define STATE_GAME_ENDED             10

#define GAME_TOP                     0
#define GAME_BOTTOM                  64
#define GAME_LEFT                    0
#define GAME_RIGHT                   128

#define STAGE_TO_START_WITH          1
#define PLAYER_CAN_DIE               1

#define SCORE_SMALL_FONT             0
#define SCORE_BIG_FONT               1

#define MAX_ONSCREEN_ENEMIES         9
#define MAX_ENEMY_BULLETS            3
#define MAX_BOSS_BULLETS             6

Arduboy2Base arduboy;
Sprites sprites;
ArduboyTones sound(arduboy.audio.enabled);

uint8_t gameState = STATE_MENU_INTRO;    // start the game with the TEAM a.r.g. logo
uint8_t menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
uint8_t globalCounter = 0;
uint8_t stage = STAGE_TO_START_WITH -1;
unsigned long scorePlayer; 
uint8_t backgroundIsVisible;
uint8_t gameOverAndStageFase;
uint8_t currentWave;
uint8_t previousWave;
int leftX;
uint8_t rightX;
uint8_t powerUpSelectorY = 0;

#endif
