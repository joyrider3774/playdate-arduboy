#pragma once
#include <Arduboy2.h>

class Records {
  public:
    uint8_t  edgeRaceMinutes;
    uint8_t  edgeRaceSeconds;
    uint32_t edgeRaceSeed;
    uint16_t crazyCourierScore;
    uint32_t crazyCourierSeed;
    uint8_t  checksum;

    void    validate();
    bool    setER(uint32_t seed, uint8_t minutes, uint8_t seconds);
    bool    setCC(uint32_t seed, uint16_t score);
    uint8_t calcChecksum();    
};
