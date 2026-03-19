#pragma once
#include "globals.h"
#include "sounds.h"
#include "levels.h"
#include "draw.h"
#include "player.h"

// Forward declarations
void resetVariables();
void refreshLevel();
void gameOver();
void winGame();

void resetLevel()
{
  for (x = 0; 16 > x; x++)
    for (y = 0; 8 > y; y++)
      screen[x][y] = 0;

  for (i = 0; dripNumber > i; i++) dripLife[i] = 0;
  for (i = 0; i > (int)cinderNumber; i++) cinderLife[i] = 0;
  levelDelay = 20;
  levelRead = 0;
  playerXvel = 0;
  playerYvel = 0;
}

void cinderSpawn()
{
  for (i = 0; cinderNumber > i; i++)
  {
    if (cinderLife[i] == 0)
    {
      r = rand() % 2;
      if (r == 0)
      {
        cinderX[i] = (playerX+1) + rand() % 5 - 2;
        cinderY[i] = (playerY+5) + rand() % 5 - 2;
        cinderLife[i] = 10;
      }
    }
  }
}

void cinderMove()
{
  if (anim3 == 2)
  {
    for (i = 0; cinderNumber > i; i++)
    {
      if (cinderLife[i] > 0)
      {
        r = rand() % 4 + 1;
        if (r == 1) cinderX[i] = cinderX[i] + 1;
        if (r == 2) cinderX[i] = cinderX[i] - 1;
        if (r == 3) cinderY[i] = cinderY[i] + 1;
        if (r == 4) cinderY[i] = cinderY[i] - 1;
        cinderLife[i]--;
      }
    }
  }
}

void dripSpawn()
{
  for (i = 0; dripNumber > i; i++)
  {
    if (dripLife[i] == 0)
    {
      r = rand() % 10;
      if (r == 0)
      {
        dripX[i] = x*8;
        dripY[i] = y*8;
        dripLife[i] = 50;
      }
    }
  }
}

void dripMove()
{
  for (i = 0; dripNumber > i; i++)
  {
    if (dripLife[i] > 0)
    {
      dripY[i] = dripY[i] + 1;
      Rect playerCol(playerX, playerY, 5, 5);
      Rect dripCol(dripX[i], dripY[i], 7, 8);
      if (arduboy.collide(playerCol, dripCol)) playerSnuff();
      dripLife[i]--;
    }
  }
}

void flameSpawn()
{
  for (i = 0; flameNumber > i; i++)
  {
    if (flameLife[i] == 0)
    {
      r = rand() % 2;
      if (r == 0)
      {
        flameX[i] = (playerX+1) + rand() % 5 - 2;
        flameY[i] = (playerY+2) + rand() % 5 - 2;
        flameLife[i] = 20;
      }
    }
  }
}

void flameMove()
{
  for (i = 0; flameNumber > i; i++)
  {
    if (flameLife[i] > 0)
    {
      if (flameX[i] < playerX - 50) flameX[i] = (playerX+1) + rand() % 5 - 2;
      if (flameX[i] > playerX + 50) flameX[i] = (playerX+1) + rand() % 5 - 2;
      if (flameY[i] < playerY - 25) flameY[i] = (playerY) + rand() % 5 - 2;
      if (flameY[i] > playerY + 25) flameY[i] = (playerY) + rand() % 5 - 2;
      flameY[i] = flameY[i] - 0.2;
      flameLife[i]--;
    }
  }
}

void animations()
{
  if (levelDelay > 0) levelDelay--;

  anim2++;
  if (anim2 == 3) anim2 = 2;

  if (anim2 == 2)
  {
    anim3++;
    if (anim3 == 4) anim3 = 1;
  }

  if (anim3 == 3)
  {
    anim6++;
    if (anim6 == 7) anim6 = 1;
  }

  if (anim2 == 2)
  {
    anim4++;
    if (anim4 == 5) anim4 = 1;
  }

  anim33++;
  if (anim33 == 34) anim33 = 1;

  if (playerJump > 0) playerJump--;
  if (playerDash > 0) playerDash--;
  if (moveRight > 0)  moveRight--;
  if (moveLeft > 0)   moveLeft--;
}

void friction()
{
  if (direction == 1)
  {
    if (moveRight < 6) playerXvel = 1;
    if (moveRight == 0) playerXvel = 0;
  }
  if (direction == 2)
  {
    if (moveLeft < 6) playerXvel = -1;
    if (moveLeft == 0) playerXvel = 0;
  }
}

void cameraShake()
{
  r = rand() % 4 + 1;
  if (r == 1) xOffset = xOffset + 1;
  if (r == 2) xOffset = xOffset - 1;
  if (r == 3) yOffset = yOffset + 1;
  if (r == 4) yOffset = yOffset - 1;
}

void flipLevel()
{
  resetLevel();

  for (i = 0; i > (int)cinderNumber; i++)
    cinderX[i] = -20;

  if (s == 1) sound.tones(doorTune);

  if (newLevel == 1)  levelOne();
  if (newLevel == 2)  levelTwo();
  if (newLevel == 3)  levelThree();
  if (newLevel == 4)  levelFour();
  if (newLevel == 5)  levelFive();
  if (newLevel == 6)  levelSix();
  if (newLevel == 7)  levelSeven();
  if (newLevel == 8)  levelEaight();
  if (newLevel == 9)  levelNine();
  if (newLevel == 10) levelTen();
  if (newLevel == 11) levelEleven();
  if (newLevel == 12) levelTwelve();
  if (newLevel == 13) levelThirteen();
  if (newLevel == 14) levelFourteen();
  if (newLevel == 16) levelSixteen();
  if (newLevel == 17) levelSeventeen();
}

void manageLevels()
{
  x = (playerX+3)/8;
  y = (playerY+3)/8;

  if (level == 1)
  {
    if (x == 15 && y == 6) { newLevel = 2; playerX = 10; flipLevel(); }
  }
  if (level == 2)
  {
    if (x == 0 && y == 6)  { newLevel = 1;  playerX = 114; flipLevel(); }
    if (x == 15 && y == 3) { newLevel = 3;  playerX = 10;  flipLevel(); }
  }
  if (level == 3)
  {
    if (x == 0 && y == 3)   { newLevel = 2; playerX = 114; flipLevel(); }
    if (x == 14 && y == 7)  { newLevel = 4; playerY = 11;  flipLevel(); }
  }
  if (level == 4)
  {
    if (x == 14 && y == 0) { newLevel = 3;  playerY = 45;  flipLevel(); }
    if (x == 0  && y == 2) { newLevel = 5;  playerX = 115; flipLevel(); }
    if (x == 1  && y == 7) { newLevel = 6;  playerY = 10;  flipLevel(); }
    if (x == 15 && y == 6) { newLevel = 12; playerX = 10;  flipLevel(); }
  }
  if (level == 5)
  {
    if (x == 0  && y == 2) { newLevel = 16; playerX = 115; flipLevel(); }
    if (x == 15 && y == 2) { newLevel = 4;  playerX = 10;  flipLevel(); }
  }
  if (level == 6)
  {
    if (x == 1  && y == 0) { newLevel = 4; playerY = 45;  flipLevel(); }
    if (x == 15 && y == 5) { newLevel = 7; playerX = 11;  flipLevel(); }
  }
  if (level == 7)
  {
    if (x == 0  && y == 5) { newLevel = 6; playerX = 115; flipLevel(); }
    if (x == 15 && y == 1) { newLevel = 8; playerX = 11;  flipLevel(); }
  }
  if (level == 8)
  {
    if (x == 0  && y == 1) { newLevel = 7; playerX = 115; flipLevel(); }
    if (x == 15 && y == 6) { newLevel = 9; playerX = 11;  flipLevel(); }
  }
  if (level == 9)
  {
    if (x == 0  && y == 6)  { newLevel = 8;  playerX = 115; flipLevel(); }
    if (x == 11 && y == 7)  { newLevel = 10; playerY = 11;  flipLevel(); }
  }
  if (level == 10)
  {
    if (x == 12 && y == 0) { newLevel = 9;  playerY = 45; flipLevel(); }
    if (x == 2  && y == 7) { newLevel = 11; playerY = 11; flipLevel(); }
  }
  if (level == 11)
  {
    if (x == 2 && y == 0) { newLevel = 10; playerY = 45; flipLevel(); }
    if (x == 13 && y == 6)
    {
      if (furnaceLit1 == 0)
      {
        if (s == 1) sound.tones(fireTune);
        furnaceLit1 = 1;
      }
    }
  }
  if (level == 12)
  {
    if (x == 0  && y == 6)  { newLevel = 4;  playerX = 115; flipLevel(); }
    if (x == 14 && y == 0)  { newLevel = 13; playerY = 45;  flipLevel(); }
  }
  if (level == 13)
  {
    if (x == 14 && y == 7) { newLevel = 12; playerY = 11; flipLevel(); }
    if (x == 7  && y == 0) { newLevel = 14; playerY = 45; flipLevel(); }
  }
  if (level == 14)
  {
    if (x == 7 && y == 7) { newLevel = 13; playerY = 11; flipLevel(); }
    if (x == 2 && y == 6)
    {
      if (furnaceLit2 == 0)
      {
        if (s == 1) sound.tones(fireTune);
        furnaceLit2 = 1;
      }
    }
  }
  if (level == 16)
  {
    if (x == 15 && y == 2) { newLevel = 5;  playerX = 11; flipLevel(); }
    if (x == 9  && y == 7) { newLevel = 17; playerY = 11; flipLevel(); }
  }
  if (level == 17)
  {
    if (x == 9 && y == 0) { newLevel = 16; playerY = 45; flipLevel(); }
    if (x == 2 && y == 6)
    {
      if (furnaceLit3 == 0)
      {
        if (s == 1) sound.tones(fireTune);
        furnaceLit3 = 1;
      }
    }
  }
}

void refreshLevel()
{
  resetLevel();
  if (level == 1)  { playerX = 27;  playerY = 27; levelOne(); }
  if (level == 2)  { playerX = 11;  playerY = 11; levelTwo(); }
  if (level == 3)  { playerX = 11;  playerY = 11; levelThree(); }
  if (level == 4)  { playerX = 11;  playerY = 11; levelFour(); }
  if (level == 5)  { playerX = 114; playerY = 11; levelFive(); }
  if (level == 6)  { playerX = 11;  playerY = 11; levelSix(); }
  if (level == 7)  { playerX = 11;  playerY = 11; levelSeven(); }
  if (level == 8)  { playerX = 11;  playerY = 11; levelEaight(); }
  if (level == 9)  { playerX = 11;  playerY = 11; levelNine(); }
  if (level == 10) { playerX = 97;  playerY = 11; levelTen(); }
  if (level == 11) { playerX = 11;  playerY = 11; levelEleven(); }
  if (level == 12) { playerX = 11;  playerY = 11; levelTwelve(); }
  if (level == 13) { playerX = 11;  playerY = 11; levelThirteen(); }
  if (level == 14) { playerX = 114; playerY = 11; levelFourteen(); }
  if (level == 16) { playerX = 115; playerY = 11; levelSixteen(); }
  if (level == 17) { playerX = 100; playerY = 11; levelSeventeen(); }
}

void resetVariables()
{
  start = 0;
  direction = 1;
  gameProgress = 0;
  introStep = 0;
  introCounter = 0;
  levelRead = 0;
  playerX = 0;
  playerY = 0;
  playerXvel = 0;
  playerYvel = 0;
  playerJump = 0;
  playerDash = 0;
  jumpPressed = 0;
  dashPressed = 0;
  playerDeath = 0;
  headBump = 0;
  leftWall = 0;
  rightWall = 0;
  fall = 0;
  moveRight = 0;
  moveLeft = 0;
  yHeight = 5;
  lives = 3;
  furnaceLit1 = 0;
  furnaceLit2 = 0;
  furnaceLit3 = 0;
  winDelay = 0;
  xOffset = 0;
  yOffset = 0;
}

void playerSnuff()
{
  if (s == 1) sound.tones(snuffTune);
  arduboy.clear();
  arduboy.display();
  arduboy.delayShort(1000);
  refreshLevel();
  for (i = 0; dripNumber > i; i++) dripX[i] = -20;
  for (i = 0; i > (int)flameNumber; i++) flameLife[i] = 0;
  for (i = 0; i > (int)cinderNumber; i++) cinderX[i] = -20;
  lives--;
  livesScreen();
  if (lives == 0) gameOver();
  playerXvel = 0;
  playerYvel = 0;
}

void gameOver()
{
  arduboy.clear();
  Sprites::drawOverwrite(0, 0, gameOverScreen, 0);
  arduboy.display();
  arduboy.delayShort(2000);
  resetVariables();
}

void winGame()
{
  arduboy.clear();
  Sprites::drawOverwrite(0, 0, win1, 0);
  arduboy.display();
  arduboy.delayShort(3000);
  resetVariables();
}

void newGame()
{
  playerX = 25;
  playerY = 27;
  direction = 1;
  if (s == 1) sound.tones(startTune);
  arduboy.delayShort(1000);
  livesScreen();
  levelOne();
}

void gameUnfold()
{
  if (winDelay == 0)
  {
    if (furnaceLit1 == 1 && furnaceLit2 == 1 && furnaceLit3 == 1)
      winDelay = 200;
  }

  if (winDelay > 0)
  {
    winDelay--;
    if (winDelay == 1) winGame();
  }

  if (touchdown > 0)
  {
    cameraShake();
    touchdown--;
    if (touchdown == 0) { xOffset = 0; yOffset = 0; }
  }

  manageLevels();
  friction();
  getInput();
  flameSpawn();
  movePlayer();
  flameMove();
  cinderMove();
  dripMove();
}
