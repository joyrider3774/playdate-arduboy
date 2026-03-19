#pragma once
#include <Arduboy2.h>
#include <ArduboyTones.h>

// Library instances
extern Arduboy2Base arduboy;
extern Sprites sprites;
extern ArduboyTones sound;

// General variables
extern bool firstRun;
extern bool start;
extern bool s;          // Sound on/off
extern int i;
extern int c;
extern int direction;
extern int r;
extern int gameProgress;
extern int introStep;
extern int introCounter;
extern int anim2;
extern int anim3;
extern int anim4;
extern int anim6;
extern int anim33;
extern int levelDelay;
extern double gravity;
extern uint8_t levelRead;
extern int level;
extern int newLevel;
extern int newX;
extern int newY;
extern bool furnaceLit1;
extern bool furnaceLit2;
extern bool furnaceLit3;
extern int winDelay;

// Player variables
extern int playerX;
extern int playerY;
extern int playerXvel;
extern double playerYvel;
extern uint8_t playerJump;
extern uint8_t playerDash;
extern bool jumpPressed;
extern bool dashPressed;
extern uint8_t touchdown;
extern uint8_t playerDeath;
extern int playerXsimp;
extern int playerYsimp;
extern bool headBump;
extern bool leftWall;
extern bool rightWall;
extern bool land;
extern int fall;
extern uint8_t moveRight;
extern uint8_t moveLeft;
extern uint8_t yHeight;
extern uint8_t lives;

// Screen variables
extern uint8_t screen[16][8];
extern uint8_t x;
extern uint8_t y;
extern int xOffset;
extern int yOffset;
extern int windowXmin;
extern int windowXmax;
extern int windowYmin;
extern int windowYmax;
extern int blockType;

// Flame variables
extern uint8_t flameNumber;
extern uint8_t flameLife[11];
extern double flameX[11];
extern double flameY[11];

// Cinder variables
extern uint8_t cinderNumber;
extern uint8_t cinderLife[31];
extern int cinderX[31];
extern int cinderY[31];

// Drip variables
extern uint8_t dripNumber;
extern uint8_t dripLife[5];
extern int dripX[5];
extern int dripY[5];
