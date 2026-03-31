/*
   Copyright (C) 2016 Ignacio Vina (@igvina)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "balls.h"
#include "extra.h"
#include "help.h"
#include "gifts.h"
#include "config.h"
#include "backgrounds.h"
#include "levels.h"
#include "game.h"
#include "menu.h"
#include "utils.h"
#include "logo.h"
#include "credits.h"
#include "globals.h"
#include "Customlib.h"

#include <EEPROM.h>
void setupEEPROM();
// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
#ifdef LOW_MEMORY
#ifdef ARDUBOY_2
  arduboy.boot();
  arduboy.audio.begin();
  tunes = new ArduboyPlaytune(arduboy.audio.enabled);
  tunes->initChannel(1);
#else
  arduboy.beginNoLogo();
#endif
  //Fake delay, use own logo
  arduboy.clear();
  helper.display();
  customDelay(500);
#else
  arduboy.begin();
#endif

  arduboy.initRandomSeed();

  setLogoMode();

  //Load values, continue level, lives...
  setupEEPROM();

}


void loop() {

  if (!(arduboy.nextFrame()))
    return;
  // To get just pressed keys
  helper.poll();

  // first we clear our screen to black
  arduboy.clear();

  if (mode == MODE_LOGO) {
    playLogo();
  } else if (mode == MODE_MENU) {
    playMenu();
  } else if (mode == MODE_HELP) {
    playHelp();
  } else if (mode == MODE_CREDITS) {
    playCredits();
  } else playGame();

  // Paint screen (not needed on game mode)
  if (mode != MODE_GAME) helper.display();
}


void setupEEPROM() {

  //Check if mem is not from game -> reset mem with default values
  if (EEPROM.read(EEPROM_GAME_IDU) != (GAME_ID + GAME_VERSION)) {
    EEPROM.write(EEPROM_GAME_IDU, (GAME_ID + GAME_VERSION));
    EEPROM.write(EEPROM_LAST_LEVEL, 0);
    EEPROM.write(EEPROM_LAST_LEVEL, 0);
    EEPROM.write(EEPROM_DIFFICULTY, DIFFICULTY_NORMAL);
    EEPROM.write(EEPROM_SOUND_ON, arduboy.audio.enabled());
    EEPROM.write(EEPROM_IMMORTAL, false);
    EEPROM.write(EEPROM_AUTOSAVE, true);
    EEPROM.write(EEPROM_FPS, FPS_30);
    EEPROM.write(EEPROM_SHOW_FPS, false);
  }

  level = EEPROM.read(EEPROM_LAST_LEVEL);

  if (level > 0) {
    // Do better to avoid this hack on start of loading game
    lives = EEPROM.read(EEPROM_LAST_LIVES);
    levelLoadedFromEEPROM = true;
    game = GAME_RUNNING;
  } else {
    levelLoadedFromEEPROM = false;
  }

  difficulty = EEPROM.read(EEPROM_DIFFICULTY);
  immortal = EEPROM.read(EEPROM_IMMORTAL);
  autosave = EEPROM.read(EEPROM_AUTOSAVE);
  sound = EEPROM.read(EEPROM_SOUND_ON);
  frameDrop = EEPROM.read(EEPROM_FPS);
  showFPS = EEPROM.read(EEPROM_SHOW_FPS);

}

