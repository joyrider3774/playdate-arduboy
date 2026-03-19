#pragma once

#include <stdint.h>
#include <Arduboy2.h>
#include "Enums.h"
#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Utils.h"
#include "src/utils/EnemyNames.h"
#include "src/levels/Level.h"
#include "images/Images.h"
#include "src/entities/Player.h"
#include "src/entities/Enemy.h"
#include "src/entities/Item.h"
#include "src/levels/MapData.h"
#include "src/controllers/PlayerController.h"
#include "src/controllers/EnemyController.h"
#include "src/fonts/Font3x5.h"
#include "src/utils/Utils.h"
#include "src/utils/EnemyNames.h"
#include "DarkUnder_Initialise.h"
#include "DarkUnder_Inventory.h"
#include "DarkUnder_Render.h"
#include "DarkUnder_Battle.h"
#include "DarkUnder_Item.h"
#include "DarkUnder_LargeMap.h"

extern Arduboy2Ext arduboy;
extern Font3x5 font3x5;

extern Item items[NUMBER_OF_ITEMS];
extern Item doors[NUMBER_OF_DOORS];
extern Enemy enemies[NUMBER_OF_ENEMIES];

extern uint8_t attackingEnemyIdx;

extern const uint8_t *levels[];
extern const uint8_t *map_images[];

// Inventory settings ..

extern const uint8_t *inventory_images[];
extern const Point inventory_Coords[];
extern uint8_t inventory_selection;
extern uint8_t inventory_action;

// Enemy details ..

extern const uint8_t *enemy_images[];
extern const uint8_t *enemy_masks[];

// Item details ..

extern const uint8_t *item_images[];
extern uint8_t itemAction;
extern uint8_t savedItem;

extern GameState gameState;
extern GameState savedState;

extern Level myLevel;
extern Player myHero;

extern SplashButtons splashStatus;
extern FightButtons fightButton;
#ifdef LEVEL_UP_SELECT_PRIZE
extern LevelUpButtons levelUpButton;
#endif

extern uint8_t level;
extern uint8_t playerLevel;

// Frame drawing
void drawFrames();

// Level Up Handler
uint16_t displayLevelUp();

// Next Level Handler
void displayNextLevel();

// End of Game Handler
void displayEndOfGame(bool playerDead);
