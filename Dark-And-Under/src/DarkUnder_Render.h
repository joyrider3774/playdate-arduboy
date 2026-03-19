#pragma once

#include <stdint.h>
#include "src/entities/Player.h"
#include "src/levels/Level.h"

// Render the first-person dungeon view, enemies, and items in front of the player
void drawPlayerVision(Player *myHero, Level *myLevel);

// Render the minimap and the HP/AP/DF/XP statistics panel
#ifdef USE_LARGE_MAP
void drawMapAndStatistics(Player *player, Level *myLevel, boolean smallMap);
#else
void drawMapAndStatistics(Player *player, Level *myLevel);
#endif

// Print a labelled statistic value (label from flash, value as uint8_t)
void printStatistic(const char *str, const uint8_t stat);

// Draw the compass / direction indicator sprite
void drawDirectionIndicator(Player *myHero);

// Draw the two-line level name description
void drawLevelDescription(Level *level);

// Draw the enemy hit-points bar in the battle view
void drawEnemyHitPointsBar(uint8_t hitPoints, uint8_t hitPointsMax);

// Render the splash / title screen
void displaySplash();

// Render the Garage Collective credits / logo screen
void displayLogo();
