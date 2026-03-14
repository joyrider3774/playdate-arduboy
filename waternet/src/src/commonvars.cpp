#include <Arduboy2.h>
#include <Arduboy2Audio.h>
#include <ArduboyTones.h>
#include <stdint.h>

#include "commonvars.h"

uint8_t startPos, menuPos, 
        maxLevel, selectedLevel, boardX, boardY, difficulty, 
        gameState, boardWidth, boardHeight, boardSize,
        levelDone, titleStep, gameMode, posAdd, debugMode, 
        mainMenu, option, needRedraw;
        
int16_t selectionX, selectionY;
uint16_t moves;
unsigned char level[maxBoardSize];
unsigned long randomSeedGame;
Arduboy2Base arduboy;
ArduboyTones arduboySound(arduboy.audio.enabled);
