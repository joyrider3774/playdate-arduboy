#ifndef STAGE_H
#define STAGE_H

#include <Arduboy2Common.h>
#include "globals.h"

uint8_t fireFrame = 0;


void drawSurrounding()
{
  for (uint8_t y = 0; y < 8; y++)
  {
    for (uint8_t x = 0; x < 16; x++)
    {
      sprites.drawPlusMask(x * 8, y * 8, surrounding_plus_mask, pgm_read_byte(&surrounding_tiles[x + 16 * y]));
    }
  }
}


void drawBackDrop()
{
  for (uint8_t y = 0; y < 6; y++)
  {
    for (uint8_t x = 0; x < 20; x++)
    {
      sprites.drawPlusMask((x * 8) + 8 - (player.x / 4) , 16 + y * 8, backdrop_plus_mask, pgm_read_byte(&backdrop_tiles[x + 20 * y]));
    }
  }
}


void drawFirePit()
{
  if (arduboy.everyXFrames(2))fireFrame++;
  if (fireFrame > 14)fireFrame = 0;
  sprites.drawPlusMask(56, 48, firepit_plus_mask, fireFrame);
}

#endif
