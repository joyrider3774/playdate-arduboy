#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

extern uint8_t checkDeath(uint8_t index);
extern void killBall(uint8_t index);
extern void deactivateDead();
extern void checkRoots();
extern uint8_t checkSurrounding(uint8_t index);

class Ball {
public:
  uint8_t state; // state of the ball position
  // 0000 0000
  // |||| |||└-\
  // |||| ||└--\ Type of ball, the sub-image. (6 is dead/falling)
  // |||| |└---/
  // |||| └----/
  // |||└------- Active
  // ||└-------- Connect to root during root expansion
  // |└--------- Unused
  // └---------- Unused
};

uint8_t launcherAngle = 90;
Ball balls[TOTAL_BALLS];

void setBallType(uint8_t ballIndex, uint8_t type) {
  if (type > 6)
    type = 0;
  balls[ballIndex].state &= 0xF0;
  balls[ballIndex].state |= (type & 0x0F);
}

uint8_t getBallType(uint8_t ballIndex) {
  return (balls[ballIndex].state & 0x0F);
}

uint8_t getBallIndex(uint8_t x, uint8_t y) {
  if (x < GAME_LEFT + 3 || x >= GAME_RIGHT - 4)
    return 255;
  uint8_t row = (y) / 5; // 5 is the vertical separation of balls
  uint8_t col;
  if (row % 2 == alignType) {
    col = (x - (GAME_LEFT + 3)) / 6; // 6 is the horizontal separation of balls
    if (col >= TOTAL_COLUMNS - 1)
      return 255;
  }
  else
    col = (x - (GAME_LEFT + 3)) / 6;
  if (row >= 11 || col >= TOTAL_COLUMNS)
    return 255;
  return (col + row * TOTAL_COLUMNS);
}

uint8_t getBall(uint8_t row, uint8_t col) {
  if (row >= 11 || col >= TOTAL_COLUMNS)
    return 255;

  if (row % 2 == alignType && col >= TOTAL_COLUMNS - 1)
    return 255;

  return (col + row * TOTAL_COLUMNS);
}

void fallingBalls() {
  if (falling) {
    if (fallOffset < 64) {
      fallOffset += 2;
    }
    else {
      falling = false;
      fallOffset = 0;
      deactivateDead();
    }
  }
}

void updateMovingBall() {
  if (aBall != 255) {
    if (aBallX <= GAME_LEFT + 5 || aBallX >= GAME_RIGHT - 4) {
      aBallRad = PI - aBallRad; // reflect ball
    }
    aBallX += cos(aBallRad) * aBallSpeed;
    aBallY -= sin(aBallRad) * aBallSpeed;
    //uint8_t forwardIndex = getBallIndex((uint8_t)(aBallX + cos(aBallRad) * 1.0), (uint8_t)(aBallY - sin(aBallRad) * 1.0));
    float abr = aBallRad;
    if (aBallX <= GAME_LEFT + 5 || aBallX >= GAME_RIGHT - 4) {
      abr = PI - abr; // reflect ball
    }
    uint8_t forwardIndex[4];
    forwardIndex[3] = getBallIndex((uint8_t)(aBallX  + 1), (uint8_t)(aBallY + 0));
    forwardIndex[2] = getBallIndex((uint8_t)(aBallX  - 1), (uint8_t)(aBallY + 0));
    forwardIndex[1] = getBallIndex((uint8_t)(aBallX  + 0), (uint8_t)(aBallY - 1));
    forwardIndex[0] = getBallIndex((uint8_t)(aBallX  + 0), (uint8_t)(aBallY + 1));
    uint8_t fd = 255;
    for (uint8_t i = 3; i < 4; i--) {
      if (forwardIndex[i] != 255 && bitRead(balls[forwardIndex[i]].state, ACTIVE_BIT))
        fd &= forwardIndex[i];
    }
    if (fd != 255) {
      uint8_t indx = getBallIndex((uint8_t)aBallX, (uint8_t)aBallY);
      setBallType(indx, aBall);
      bitSet(balls[indx].state, ACTIVE_BIT);
      bitClear(balls[indx].state, ROOT_BIT);
      aBall = 255;
      if (checkSurrounding(indx)) {
        falling = true;
        checkRoots();
      }
    }

    if (aBallY <= 3) {
      uint8_t indx = getBallIndex((uint8_t)aBallX, (uint8_t)aBallY);
      if (indx != 255) {
        setBallType(indx, aBall);
        bitSet(balls[indx].state, ACTIVE_BIT);
        bitClear(balls[indx].state, ROOT_BIT);
      }
      aBall = 255;
      if (checkSurrounding(indx)) {
        falling = true;
        checkRoots();
      }
    }
  }
}

void drawBalls() {
  for (uint8_t i = TOTAL_BALLS-1; i < TOTAL_BALLS; i--) {
    if (bitRead(balls[i].state, ACTIVE_BIT)) {
      uint8_t col = i % TOTAL_COLUMNS;
      uint8_t row = i / TOTAL_COLUMNS;
      sprites.drawPlusMask(GAME_LEFT + 2 + col * 6 + ((row % 2 == alignType) ? 3 : 0), 5 * row + ((getBallType(i) == 6) ? fallOffset : 0), ballMask_plus_mask, 0);
      sprites.drawErase(GAME_LEFT + 3 + col * 6 + ((row % 2 == alignType) ? 3 : 0), 5 * row + 1 + ((getBallType(i) == 6) ? fallOffset : 0), sprBalls, getBallType(i));
    }
  }
}

void fillBallQueue() {
  for (uint8_t i = 0; i < 6; i++) {
    ballQueue[i] = generateRandomNumber(6);
  }
}

void shiftBallQueue(bool newball) {
  for (uint8_t i = 0; i < 5; i++) {
    ballQueue[i] = ballQueue[i + 1];
  }
  if (newball)
    ballQueue[5] = generateRandomNumber(6);
  else
    ballQueue[5] = 255;
}

void drawBallQueue() {
  for (uint8_t i = 1; i < 6; i++) {
    if (ballQueue[i] != 255)
      sprites.drawPlusMask(LAUNCHER_X - 7 - (i - 1) * 6, 58, ballMask_plus_mask, 0);
      sprites.drawErase(LAUNCHER_X - 6 - (i - 1) * 6, 59, sprBalls, ballQueue[i]);
  }
}

void shiftBallsDown(bool fillTopRow) {
  alignType ^= 1;
  for (uint8_t i = 10; i > 0; i--) { // row
    for (uint8_t j = TOTAL_COLUMNS - 1; j < TOTAL_COLUMNS; j--) { // column
      balls[j + i * TOTAL_COLUMNS] = balls[j + (i - 1) * TOTAL_COLUMNS];
    }
  }
  if (fillTopRow) {
    for (uint8_t i = TOTAL_COLUMNS-1; i < TOTAL_COLUMNS; i--) {
      balls[i].state = generateRandomNumber(6);
      bitSet(balls[i].state, ACTIVE_BIT);
      bitSet(balls[i].state, ROOT_BIT);
    }
    if (!alignType)
      bitClear(balls[TOTAL_COLUMNS-1].state, ACTIVE_BIT); 
  }
  else {
    for (uint8_t i = TOTAL_COLUMNS-1; i < TOTAL_COLUMNS; i--) {
      bitClear(balls[i].state, ACTIVE_BIT);
    }
  }
}

bool checkBottomRow() {
  for (uint8_t i = TOTAL_BALLS-1; i >= TOTAL_BALLS-12; i--) {
    if (bitRead(balls[i].state, ACTIVE_BIT))
      return true;
  }
  return false;
}

void clearBalls() {
  for (uint8_t i = TOTAL_BALLS-1; i < TOTAL_BALLS; i--) {
    balls[i].state = 0;
  }
}

#endif
