#include <stdint.h>

#include "commonvars.h"
#include "helperfuncs.h"

#include "../res/blocktiles.h"

const unsigned char *currentTiles = nullptr;

void set_bkg_tile_xy(uint8_t x, uint8_t y, uint8_t tile)
{
  if(currentTiles != nullptr)
    arduboy.drawBitmap(x * 8, y * 8, &currentTiles[2 + (tile * 8)] , 8, 8);
}

void set_bkg_data(const unsigned char *tiles)
{
  currentTiles = tiles;
}

const unsigned char * get_bkg_data()
{
  return currentTiles;
}

void set_bkg_tiles(uint8_t x, uint8_t y, const uint8_t *map)
{
  arduboy.drawBitmap(x, y,  &map[2], pgm_read_byte(&map[0]), pgm_read_byte(&map[1]));
}

void setBlockTilesAsBackground()
{
  set_bkg_data(blockTiles);
}