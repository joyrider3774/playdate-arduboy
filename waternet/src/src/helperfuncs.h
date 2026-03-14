#ifndef helperfuncs_h
#define helperfuncs_h

#include <stdint.h>

void setBlockTilesAsBackground();
void set_bkg_tile_xy(uint8_t x, uint8_t y, uint8_t tile);
void set_bkg_data(const unsigned char *tiles);
const unsigned char* get_bkg_data();
void set_bkg_tiles(uint8_t x, uint8_t y,const uint8_t *map);
#endif