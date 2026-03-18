#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

constexpr uint8_t musGame = 1;
constexpr uint8_t musTitle = 2;
constexpr uint8_t musLevelClear = 3;
constexpr uint8_t musAllLevelsClear = 4;

constexpr uint8_t sfxSustain = (100 * frameRate) / 1000;

void initSound();
void SelectMusic(uint8_t musicFile, uint8_t force = 0);
void initMusic();
void soundTimer();
void playGameAction();
void playMenuAcknowlege();
void playMenuBackSound();
void playMenuSelectSound();
void playErrorSound();
void playGameMoveSound();
void setMusicOn(uint8_t value);
void setSoundOn(uint8_t value);
uint8_t isMusicOn();
uint8_t isSoundOn();


#endif