#pragma once

#include <stdint.h>
#include "src/utils/Structs.h"

void    initEEPROM(bool forceOverwrite);
int16_t checkSum(bool update);
void    checkHighScoreSlot(uint16_t score);
void    writeHighScoreEntry(HighScoreVars &highScoreVars);
void    highScores();
