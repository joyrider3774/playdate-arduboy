#pragma once
#include "globals.h"
#include "sounds.h"

// Forward declarations
void cinderSpawn();
void playerSnuff();

void wallBurnRight()
{
  if (anim33 == 11)
  {
    if (screen[((playerX+5)/8)][((playerY+3)/8)] == 6)
      screen[((playerX+5)/8)][((playerY+3)/8)] = 7;
  }
  if (anim33 == 22)
  {
    if (screen[((playerX+5)/8)][((playerY+3)/8)] == 7)
      screen[((playerX+5)/8)][((playerY+3)/8)] = 8;
  }
  if (anim33 == 33)
  {
    if (screen[((playerX+5)/8)][((playerY+3)/8)] == 8)
      screen[((playerX+5)/8)][((playerY+3)/8)] = 0;
  }
}

void wallBurnLeft()
{
  if (anim33 == 11)
  {
    if (screen[((playerX)/8)][((playerY+3)/8)] == 6)
      screen[((playerX)/8)][((playerY+3)/8)] = 7;
  }
  if (anim33 == 22)
  {
    if (screen[((playerX)/8)][((playerY+3)/8)] == 7)
      screen[((playerX)/8)][((playerY+3)/8)] = 8;
  }
  if (anim33 == 33)
  {
    if (screen[((playerX)/8)][((playerY+3)/8)] == 8)
      screen[((playerX)/8)][((playerY+3)/8)] = 0;
  }
}

void movePlayer()
{
  if (playerJump == 0)
  {
    if (screen[((playerX+1)/8)][((playerY+yHeight)/8)] == 0
    && screen[((playerX+3)/8)][((playerY+yHeight)/8)] == 0)
    {
      playerYvel = playerYvel + gravity;
      fall++;
    }

    if (screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] != 0
    && screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] != 9)
    {
      headBump = 0;
      playerYvel = 0;
      if (touchdown == 0 && land == 0)
      {
        touchdown = 5;
        if (s == 1) sound.tones(landTune);
      }
      fall = 0;
      land = 1;
    }

    if (screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] != 0 &&
        screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] == 6 ||
        screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] == 7 ||
        screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] == 8)
    {
      cinderSpawn();
    }

    // burn wood from top
    if (anim33 == 11)
    {
      if (screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] != 0
      && screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] == 6)
        screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] = 7;
    }
    if (anim33 == 22)
    {
      if (screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] != 0
      && screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] == 7)
        screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] = 8;
    }
    if (anim33 == 33)
    {
      if (screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] != 0
      && screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] == 8)
        screen[((playerX+3)/8)][((playerY+yHeight+1)/8)] = 0;
    }

    if (screen[((playerX+3)/8)][((playerY+2)/8)] == 9) playerSnuff();
  }
  else
  {
    if (screen[((playerX+1)/8)][((playerY+yHeight)/8)] == 0
    && screen[((playerX+3)/8)][((playerY+yHeight)/8)] == 0)
    {
      playerYvel = playerYvel + gravity;
      land = 0;
    }
  }

  if (headBump == 0 && screen[((playerX+3)/8)][((playerY)/8)] != 0)
  {
    headBump = 1;
    playerYvel = 0;
  }

  if (direction == 1 && screen[((playerX+5)/8)][((playerY+2)/8)] != 0
                     && screen[((playerX+5)/8)][((playerY+5)/8)] != 0)
  {
    playerXvel = 0;
    moveRight = 0;
    rightWall = 1;
    if (touchdown == 0 && land == 0)
    {
      touchdown = 3;
      if (s == 1) sound.tones(landTune);
    }
    land = 1;
    wallBurnRight();
  }

  if (direction == 2 && screen[((playerX)/8)][((playerY+2)/8)] != 0
                     && screen[((playerX)/8)][((playerY+5)/8)] != 0)
  {
    playerXvel = 0;
    moveLeft = 0;
    leftWall = 1;
    if (touchdown == 0 && land == 0)
    {
      touchdown = 3;
      if (s == 1) sound.tones(landTune);
    }
    land = 1;
    wallBurnLeft();
  }

  if (screen[((playerX)/8)][((playerY+2)/8)] == 0
  && screen[((playerX+5)/8)][((playerY+2)/8)] == 0)
  {
    rightWall = 0;
    leftWall = 0;
  }

  if (playerX < 0)   { playerX = 9;   playerXvel = 1; }
  if (playerX > 122) { playerX = 114; playerXvel = -1; }
  if (playerY < 0)   { playerY = 9;   playerYvel = 1; }
  if (playerY > 55)  { playerY = 50;  playerYvel = -1; }

  if (rightWall == 1 || leftWall == 1)
  {
    if (playerJump == 0) playerYvel = 0;
  }

  playerX = playerX + playerXvel;
  playerY = playerY + playerYvel;

  if (screen[(playerX+3)/8][(playerY+5)/8] != 0) playerY = playerY - 1;
}

void getInput()
{
  if (arduboy.pressed(RIGHT_BUTTON))
  {
    direction = 1;
    if (playerXvel < 1) playerXvel = playerXvel + 1;
    if (moveRight < 4)  moveRight = 4;
    moveLeft = 0;
  }

  if (arduboy.pressed(LEFT_BUTTON))
  {
    direction = 2;
    if (playerXvel > -1) playerXvel = playerXvel - 1;
    if (moveLeft < 4)    moveLeft = 4;
    moveRight = 0;
  }

  if (arduboy.pressed(B_BUTTON) && playerDash == 0 && dashPressed == 0)
  {
    cinderSpawn();
    if (s == 1) sound.tones(dashTune);
    if (direction == 1)
    {
      playerXvel = 3;
      playerDash = 10;
      dashPressed = 1;
      moveRight = 16;
      moveLeft = 0;
    }
    if (direction == 2)
    {
      playerXvel = -3;
      playerDash = 10;
      dashPressed = 1;
      moveLeft = 16;
      moveRight = 0;
    }
  }
  if (arduboy.notPressed(B_BUTTON)) dashPressed = 0;

  // wall jump
  if (rightWall == 1 || leftWall == 1)
  {
    if (arduboy.pressed(A_BUTTON) && jumpPressed == 0)
    {
      cinderSpawn();
      if (s == 1) sound.tones(jumpTune);
      playerJump = 10;
      jumpPressed = 1;
      if (rightWall == 1)
      {
        playerXvel = -1;
        playerYvel = -3;
        playerX--;
        moveRight = 20;
      }
      if (leftWall == 1)
      {
        playerXvel = 1;
        playerYvel = -3;
        playerX++;
        moveLeft = 20;
      }
    }
  }

  // normal jump
  if (rightWall == 0 && leftWall == 0)
  {
    if (jumpPressed == 0 && arduboy.pressed(A_BUTTON)
    && (screen[((playerX+2)/8)][((playerY+yHeight+2)/8)] != 0)
    && (screen[((playerX+2)/8)][((playerY)/8)] == 0))
    {
      cinderSpawn();
      if (s == 1) sound.tones(jumpTune);
      playerJump = 18;
      playerYvel = -4;
      jumpPressed = 1;
    }
  }

  if (arduboy.notPressed(A_BUTTON)) jumpPressed = 0;
}
