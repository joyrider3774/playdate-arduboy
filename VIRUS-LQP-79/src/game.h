#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "level.h"
#include "enemies.h"
#include "elements.h"
#include "bullet.h"
#include "door.h"

extern int level;
extern int displayLevel;
extern uint8_t gameID;

// methods prototypes ////////////////////////////////////////////////////////

void stateGamePlaying();
void stateGameNextLevel();
void stateGamePrepareLevel();
void stateGameOver();
void stateGamePause();
void stateGameEnd();
void checkPause();
void stateGameNew();
void stateGameContinue();
void stateGameMayhem();

#endif
