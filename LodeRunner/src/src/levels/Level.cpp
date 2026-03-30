#include "Level.h"
#include "../../globals.h"
#include "Levels.h"

void Level::loadLevel(Player *player, Enemy enemies[]) {

  uint16_t dataOffset = 0;
  uint8_t goldLeft = 0;

  const uint8_t *levelToLoad = levels[_levelNumber - LEVEL_OFFSET];
  player->setStance(PlayerStance::Running_Right1);


  // Load player starting position ..

  uint16_t  playerX = pgm_read_byte(&levelToLoad[dataOffset++]) * GRID_SIZE;
  uint16_t  playerY = pgm_read_byte(&levelToLoad[dataOffset++]) * GRID_SIZE;


  // Determine player's X Pos and level offset ..

  if (playerX < (WIDTH / 2) - 5) {

    _xOffset = 0;
    player->setX(playerX);

  }
  else {

    if (playerX >= (WIDTH / 2) - 5 && playerX <= (_width * GRID_SIZE * 2) - WIDTH) {

      player->setX((WIDTH / 2) - 5);
      _xOffset = player->getX() - playerX;


    }
    else {

      _xOffset = -153;
      player->setX(playerX + _xOffset);

    }

  }


  // Determine player's Y Pos and level offset ..

  if (playerY < (HEIGHT_LESS_TOOLBAR / 2) - 5) {

    _yOffset = 0;
    player->setY(playerY);

  }
  else {

    if (playerY >= (HEIGHT_LESS_TOOLBAR / 2) - 5 && playerY <= (_height * GRID_SIZE) - HEIGHT_LESS_TOOLBAR) {

      player->setY((HEIGHT_LESS_TOOLBAR / 2) - 5);
      _yOffset = player->getY() - playerY;

    }
    else {

      _yOffset = -(_height * GRID_SIZE) + HEIGHT_LESS_TOOLBAR;
      player->setY(playerY + _yOffset);

    }

  }


  // Load enemies ..

  uint8_t numberOfEnemies = pgm_read_byte(&levelToLoad[dataOffset++]);

  for (uint8_t x = 0; x < NUMBER_OF_ENEMIES; x++) {

    Enemy *enemy = &enemies[x];

    enemy->setId(x);
    enemy->setGoldCountdown(0);

    if (x < numberOfEnemies) {

      enemy->setX(pgm_read_byte(&levelToLoad[dataOffset++]) * GRID_SIZE);
      enemy->setY(pgm_read_byte(&levelToLoad[dataOffset++]) * GRID_SIZE);
      enemy->setEnabled(true);

    }
    else {

      enemy->setEnabled(false);

    }

  }


  // Load level ladder points ..

  _levelLadderElementCount = pgm_read_byte(&levelToLoad[dataOffset++]);

  for (uint8_t x = 0; x < _levelLadderElementCount; x++) {

    _levelLadder[x].x = pgm_read_byte(&levelToLoad[dataOffset++]);
    _levelLadder[x].y = pgm_read_byte(&levelToLoad[dataOffset++]);

  }


  // Load reentry points ..

  for (uint8_t x = 0; x < NUMBER_OF_REENTRY_POINTS; x++) {

    _reentryPoint[x].x = pgm_read_byte(&levelToLoad[dataOffset++]);
    _reentryPoint[x].y = pgm_read_byte(&levelToLoad[dataOffset++]);

  }


  // Load level data .. 

  uint8_t encryptionType = pgm_read_byte(&levelToLoad[dataOffset++]);
  
  if (encryptionType == ENCRYPTION_TYPE_GRID) {

    for (uint8_t y = 0; y < _height; y++) {

      for (uint8_t x = 0; x < _width; x++) {

        uint8_t data = pgm_read_byte(&levelToLoad[(y * _width) + x + dataOffset]);

        if (leftValue(data) == static_cast<uint8_t>(LevelElement::Gold))            { goldLeft++;}
        if (rightValue(data) == static_cast<uint8_t>(LevelElement::Gold))           { goldLeft++;}

        _levelData[x][y] = data;

      }

    }

  }
  else {
	  
    uint16_t cursor = 0;

    while (true) {

      uint8_t data = pgm_read_byte(&levelToLoad[dataOffset]);
      uint8_t block = (data & 0xE0) >> 5;
      uint8_t run = data & 0x1F;

      if (block == static_cast<uint8_t>(LevelElement::Gold))            { goldLeft = goldLeft + run;}

      if (run > 0) {

        dataOffset++;

        for (uint8_t x = 0; x < run; x++) {

          if (encryptionType == ENCRYPTION_TYPE_RLE_ROW) {
            
            uint8_t row = cursor / (_width * 2);
            uint8_t col = (cursor % (_width * 2)) / 2;

            if (cursor % 2 == 0) {
              _levelData[col][row] = (_levelData[col][row] & 0x0f) | (block << 4);
            } 
            else {
              _levelData[col][row] = (_levelData[col][row] & 0xF0) | block;
            }

          }
          else {

            uint8_t col = cursor / _height;
            uint8_t row = cursor % _height;

            if (col % 2 == 0) {
              _levelData[col / 2][row] = (_levelData[col / 2][row] & 0x0f) | (block << 4);
            } 
            else {
              _levelData[col / 2][row] = (_levelData[col / 2][row] & 0xF0) | block;
            }

          }

          cursor++;

        }

      }
      else {
      
        break;
      
      }

    }

  }

  _goldLeft = goldLeft;

}
