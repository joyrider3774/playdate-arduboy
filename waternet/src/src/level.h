#ifndef level_h
#define level_h

#include <stdint.h>

void moveBlockDown(uint8_t aTile);
void moveBlockUp(uint8_t aTile);
void moveBlockRight(uint8_t aTile);
void moveBlockLeft(uint8_t aTile);
void rotateBlock(uint8_t aTile);
void updateConnected();
void initLevel(unsigned long aRandomSeed);
uint8_t isLevelDone();

#endif