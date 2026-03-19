#pragma once

#include <stdint.h>
#include "Enums.h"

// Battle loop - returns delay in milliseconds
uint16_t battleLoop();

// Inflict damage on an enemy
void damageEnemy(uint8_t attackingEnemyIdx, uint8_t hpLoss);

// Battle state handlers - each returns the next GameState
GameState battleEnemyAttacksInit(void);
GameState battleEnemyAttacks(void);
GameState battleEnemyDies(void);
GameState battlePlayerDecides(void);
GameState battlePlayerAttacks(void);
GameState battlePlayerDefends(void);
GameState battlePlayerCastsSpell(void);
