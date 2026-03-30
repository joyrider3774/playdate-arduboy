#pragma once

#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Enums.h"
#include "src/utils/Queue.h"
#include "src/characters/Player.h"
#include "src/characters/Enemy.h"
#include "src/levels/Level.h"
#include <ArduboyTones.h>

// Global variables defined in LodeRunner.cpp
extern Arduboy2Ext arduboy;
extern ArduboyTones sound;
extern Player player;
extern Enemy enemies[NUMBER_OF_ENEMIES];
extern Level level;
extern GameState gameState;
extern bool flashPlayer;
extern int8_t introRect;
extern uint8_t suicide;
extern Queue<Hole, 20> holes;

// LodeRunner.cpp
void playerDies();
void setup();
void loop();
void Intro();
void GameSelect();
void LevelPlay();
void playerDies();
void NextGame();
void CompleteGame();
void CompleteSeries();

// LodeRunner_EnemyMovements.cpp
void clearEnemyMovementPositions(Enemy *enemies);
void enemyMovements(Enemy *enemy);
boolean isOccupiedByAnotherEnemy(Enemy *enemies, Enemy *enemy, int8_t xDelta, int8_t yDelta);
boolean attemptToMove(Enemy *enemy, uint8_t enemyX, uint8_t enemyY,
                      Direction direction,
                      LevelElement current, LevelElement up,
                      LevelElement right, LevelElement rightDown, LevelElement down,
                      LevelElement leftDown, LevelElement left, bool randomMoves);
void setDirectionAfterHoleEscape(Enemy *enemy);
void move(Enemy *enemy, int8_t x, int8_t y, bool randomMoves);
void updateEnemyStance(Enemy *enemy, PlayerStance lowerRange, PlayerStance upperRange);

// LodeRunner_PlayerMovements.cpp
void stopPlayer();
void playerMovements(uint8_t nearestX, uint8_t nearestY, LevelElement nearest);
void movePlayerLeft();
void movePlayerRight();
void movePlayerDown();
void movePlayerUp();
void updatePlayerStance(PlayerStance lowerRange, PlayerStance upperRange);

// LodeRunner_Render.cpp
void renderScreen();
void renderLevelElements();
void renderEnemies();
void renderArrows();
void renderArrows(bool smallArrows);
void renderEntryRectangle();
void renderScoreboard();

// LodeRunner_SplashScreen.cpp
void splashScreen_Init();
void splashScreen();

// LodeRunner_Utils.cpp
uint8_t getNearestX(int8_t margin = HALF_GRID_SIZE);
uint8_t getNearestY(int8_t margin = HALF_GRID_SIZE);
boolean inCellX();
boolean inCellX(uint8_t margin);
boolean inCellY();
boolean inCellY(uint8_t margin);
PlayerStance getNextStance(PlayerStance stance);
Direction getDirection_4Directions(Direction direction);
Direction getDirection_16Directions(int16_t xDiff, int16_t yDiff);
