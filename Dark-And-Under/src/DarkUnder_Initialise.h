#pragma once

#include <stdint.h>
#include "src/entities/Player.h"
#include "src/entities/Enemy.h"
#include "src/entities/Item.h"
#include "src/levels/Level.h"

// Initialise the full game state
void initialiseGame();

// Initialise a level from progmem level data
void initialiseLevel(Player *myHero, Level *myLevel, const uint8_t *level);

// Load items or doors from progmem level data; returns updated index
uint8_t loadItems(const uint8_t *level, Item *items, uint8_t idx, uint8_t max);

// Load enemies from progmem level data; returns updated index
uint8_t loadEnemies(const uint8_t *level, Enemy *enemies, uint8_t idx, uint8_t max);

#ifdef SAVE_GAME
// Initialise EEPROM storage (writes magic bytes if not present)
bool initEEPROM();

// Return the saved level number (255 = no save)
uint8_t getLevel();

// Persist current game state to EEPROM
void saveGame();

// Restore game state from EEPROM
void restoreGame();
#endif
