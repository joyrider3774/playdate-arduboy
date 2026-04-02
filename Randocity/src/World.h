#pragma once
#include <Arduboy2.h>
#include "lfsr.h"

class World {
  public:
    static const int16_t mapWidth = 256; 
    static const int16_t mapHeight = 256;
    static const uint8_t tileTypes = 16;

    static const uint8_t mainLeft = 0;
    static const uint8_t mainRight = 80;
    static const uint8_t mainTop = 0;
    static const uint8_t mainBottom = HEIGHT;
    
    uint8_t miniLeft = mainRight;
    uint8_t miniRight = WIDTH;
    uint8_t miniTop = 0;
    uint8_t miniBottom = HEIGHT;

    static const uint32_t minSeed = 1;
    static const uint32_t maxSeed = (1ul<<20)-1;

    uint32_t seed;
        
    void init(uint32_t seed);    
    void draw(int32_t x, int32_t y);
    void drawMini(int32_t x, int32_t y);
    uint8_t segmentsAt(int32_t playerX, int32_t playerY);
    int8_t tileAt(int32_t playerX, int32_t playerY);

  private:
    LFSR     lfsr;
    int16_t  lfsrX;
    int16_t  lfsrY;

    int8_t getBlock(int16_t x, int16_t y);
    int8_t peekBlock(int16_t x, int16_t y);
    int8_t blockType(int8_t tile);
    int8_t calcBlock(int16_t playerX, int16_t playerY);
    uint8_t getSegments(int8_t block);
    int8_t tileInBlock(uint8_t segments, int16_t tileX, int16_t tileY, uint8_t otherSegments);
    int8_t mergeTile(int8_t tile1, int8_t tile2);
    void drawTile(const unsigned char *sprites, uint8_t tile, uint8_t sx, uint8_t sy, uint8_t tx, uint8_t ty, uint8_t mx, uint8_t my);
};

    
