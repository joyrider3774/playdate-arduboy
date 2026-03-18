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
#define STATE_GAME_INIT_LEVEL        6
#define STATE_GAME_PLAYING           7
#define STATE_GAME_PAUSE             8
#define STATE_GAME_OVER              9

Arduboy2Base arduboy;
Sprites sprites;
ArduboyTones sound(arduboy.audio.enabled);

//determines the state of the game
uint8_t gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
//generic variable to store menuSelection (on screens)
uint8_t menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
uint8_t globalCounter = 0;

uint8_t walkingFrame;
uint8_t waitingFrame;
int scorePlayer;
#endif
