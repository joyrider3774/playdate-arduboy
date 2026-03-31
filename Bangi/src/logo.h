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
#include <Arduboy2.h>
#include "globals.h"

#define STATE_ANIMATING_LOGO    0
#define STATE_BOUNCING_BALL     1
#define STATE_ADJUSTING_LOGO    2

int logoState;


int logoOffsetY;
const int ADJUST_OFFSET_Y = 9;

int logoPosX;

int ballPosX;
int ballPosY;

int ballSpeedX;
int ballSpeedY;

//Ready to be visible
const int BALL_START_X  = 18;
const int BALL_START_Y = - SMALL_BALL_SIZE;

const int BALL_GROUND_Y = (HEIGHT + LOGO_HEIGHT) / 2  - SMALL_BALL_SIZE ;

const unsigned char LOGO_SPEED = 1;


void playLogo() {

  // Paint cropped logo
  helper.drawBitmap((WIDTH - LOGO_WIDTH) / 2, (HEIGHT - LOGO_HEIGHT) / 2 - logoOffsetY, LOGO_BITMAP, LOGO_WIDTH, LOGO_HEIGHT, logoPosX, 0);

  // Paint ball
  helper.drawMaskedBitmap(ballPosX / SCREEN_MAGNIFICATION, ballPosY / SCREEN_MAGNIFICATION - logoOffsetY,
                          SMALL_BALL_BITMAP ,
                          SMALL_BALL_MASK_BITMAP, SMALL_BALL_SIZE, SMALL_BALL_SIZE,  0);

  if (logoState == STATE_ANIMATING_LOGO) {
    if (logoPosX <= LOGO_WIDTH) {
      logoPosX += LOGO_SPEED;
    } else {
      logoState = STATE_BOUNCING_BALL;
    }
  } else if (logoState == STATE_BOUNCING_BALL) {

    //NORMAL SPEED -> loopTimes = 2
    for (int j = 0; j < 2 ; j++) {
      // Animate ball
      ballPosX += ballSpeedX;

      ballSpeedY += GRAVITY_ACCEL;
      ballPosY += ballSpeedY;

      // Check ground reboud
      if (ballPosY / SCREEN_MAGNIFICATION > BALL_GROUND_Y) {
        ballPosY = BALL_GROUND_Y  * SCREEN_MAGNIFICATION;
        ballSpeedY = - BALL_SMALL_SPEED_Y ;
      }
    }
    
    if (ballPosX / SCREEN_MAGNIFICATION > BALL_STOPS_X) {
      ballPosX = BALL_STOPS_X * SCREEN_MAGNIFICATION;
      ballPosY = ADJUST_OFFSET_Y * SCREEN_MAGNIFICATION;
      logoState = STATE_ADJUSTING_LOGO;
    }

  } else if (logoState == STATE_ADJUSTING_LOGO && arduboy.everyXFrames(2)) {
    logoOffsetY += LOGO_SPEED;
    if (logoOffsetY >= ADJUST_OFFSET_Y) {
      logoOffsetY = ADJUST_OFFSET_Y;
      setMenuMode();
    }
  }
}

void setLogoMode() {
  arduboy.clear();
  helper.display();
  customDelay(300);
  arduboy.setFrameRate(60);
  mode = MODE_LOGO;
  logoPosX = 0;
  ballPosX = BALL_START_X * SCREEN_MAGNIFICATION;
  ballPosY = BALL_START_Y * SCREEN_MAGNIFICATION;
  ballSpeedX = BALL_SMALL_SPEED_X;
  ballSpeedY = 0;
  logoOffsetY = 0;
  logoState = STATE_ANIMATING_LOGO;
}
