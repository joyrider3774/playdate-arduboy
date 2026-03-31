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

#pragma once

#include "globals.h"


extern void setMenuMode();

// Simulate old TV
const unsigned int FPS = 24;

unsigned int noise;
const unsigned int NOISE_LIMIT = 128;

void playCredits() {

  helper.drawMaskedBitmap((WIDTH - LOGO_WIDTH) / 2, (HEIGHT - LOGO_HEIGHT) / 2, LOGO_BITMAP, LOGO_BITMAP, LOGO_WIDTH, LOGO_HEIGHT, 0);
  helper.drawMaskedBitmap(92, 9, SMALL_BALL_BITMAP, SMALL_BALL_MASK_BITMAP, SMALL_BALL_SIZE, SMALL_BALL_SIZE, 0 );
  helper.print(STRING_BY_IGVINA, 35, 52);
  helper.print(STRING_GAME_VERSION, 104, 35);


  if (arduboy.everyXFrames(FPS / 2) && noise < NOISE_LIMIT) {
    noise *= 2;
  }

  helper.addNoise(noise == 1 ? 2 : noise);


  // Go to main menu if any key is pressed
  if (helper.justPressed(0xFF)) {
    setMenuMode();
  }
}


void initCredits() {
  noise = 1;
  helper.createNoiseArray(noise);
  arduboy.setFrameRate(FPS);
}

