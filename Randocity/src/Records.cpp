#include "Records.h"

void Records::validate() {
  if (checksum != calcChecksum()) {
    edgeRaceMinutes = 99;
    edgeRaceSeconds = 0;
    edgeRaceSeed = 0xFADED;
    crazyCourierScore = 0;
    crazyCourierSeed = 0xDECAF;
    checksum = calcChecksum();
  }
}

bool Records::setER(uint32_t seed, uint8_t minutes, uint8_t seconds) {
  if ((minutes < edgeRaceMinutes) || ((minutes == edgeRaceMinutes) && (seconds < edgeRaceSeconds))) {
    edgeRaceMinutes = minutes;
    edgeRaceSeconds = seconds;
    edgeRaceSeed = seed;
    checksum = calcChecksum();
    return true;
  }
  else {
    return false;
  }
}

bool Records::setCC(uint32_t seed, uint16_t score) {
  if (score > crazyCourierScore) {
    crazyCourierScore = score;
    crazyCourierSeed = seed;
    checksum = calcChecksum();
    return true;
  }
  else {
    return false;
  }
}

uint8_t Records::calcChecksum() {
  return 0x58 ^
         edgeRaceMinutes ^
         edgeRaceSeconds ^
         edgeRaceSeed ^
         (edgeRaceSeed >> 8) ^
         (edgeRaceSeed >> 16) ^
         crazyCourierScore ^
         (crazyCourierScore >> 8) ^
         crazyCourierSeed ^
         (crazyCourierSeed >> 8) ^
         (crazyCourierSeed >> 16);
}
