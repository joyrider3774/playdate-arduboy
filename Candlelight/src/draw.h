#pragma once
#include "globals.h"
#include "images.h"

void furnaceLit_draw()
{
  if (anim4 == 1) Sprites::drawOverwrite(16+xOffset, 48+yOffset, fFlame1, 0);
  if (anim4 == 2) Sprites::drawOverwrite(16+xOffset, 48+yOffset, fFlame2, 0);
  if (anim4 == 3) Sprites::drawOverwrite(16+xOffset, 48+yOffset, fFlame3, 0);
  if (anim4 == 4) Sprites::drawOverwrite(16+xOffset, 48+yOffset, fFlame4, 0);
}

void furnaceDraw()
{
  if (level == 11 && furnaceLit1 == 1)
  {
    if (anim4 == 1) Sprites::drawOverwrite(100+xOffset, 48+yOffset, fFlame1, 0);
    if (anim4 == 2) Sprites::drawOverwrite(100+xOffset, 48+yOffset, fFlame2, 0);
    if (anim4 == 3) Sprites::drawOverwrite(100+xOffset, 48+yOffset, fFlame3, 0);
    if (anim4 == 4) Sprites::drawOverwrite(100+xOffset, 48+yOffset, fFlame4, 0);
  }
  if (level == 14 && furnaceLit2 == 1) furnaceLit_draw();
  if (level == 17 && furnaceLit3 == 1) furnaceLit_draw();
}

void objectsDraw()
{
  if (level == 1)
  {
    if (24 > playerX-31 && 24 < playerX+31 && 32 > playerY-31 && 32 < playerY+10)
      Sprites::drawOverwrite(24+xOffset, 32+yOffset, candle, 0);
  }
  if (level == 11)
  {
    if (100 > playerX-25 && 100 < playerX+25 && 32 > playerY-31 && 32 < playerY+22)
      Sprites::drawOverwrite(100+xOffset, 40+yOffset, furnace1, 0);
  }
  if (level == 14)
  {
    if (16 > playerX-25 && 16 < playerX+25 && 32 > playerY-31 && 32 < playerY+22)
      Sprites::drawOverwrite(16+xOffset, 40+yOffset, furnace1, 0);
  }
  if (level == 17)
  {
    if (16 > playerX-25 && 16 < playerX+25 && 32 > playerY-31 && 32 < playerY+22)
      Sprites::drawOverwrite(16+xOffset, 40+yOffset, furnace1, 0);
  }
}

void drawMask()
{
  if (anim4 == 1) Sprites::drawExternalMask((playerX+xOffset)-30, (playerY+yOffset)-31, mask0mask, light1, 0, 0);
  if (anim4 == 2) Sprites::drawExternalMask((playerX+xOffset)-30, (playerY+yOffset)-31, mask0mask, light2, 0, 0);
  if (anim4 == 3) Sprites::drawExternalMask((playerX+xOffset)-30, (playerY+yOffset)-31, mask0mask, light3, 0, 0);
  if (anim4 == 4) Sprites::drawExternalMask((playerX+xOffset)-30, (playerY+yOffset)-31, mask0mask, light2, 0, 0);
}

void drawPlayer()
{
  Sprites::drawExternalMask(playerX+xOffset, (playerY+yOffset)+1, player0, player0mask, 0, 0);
}

// Forward declarations for dripSpawn (called from drawLevel)
void dripSpawn();

void drawLevel()
{
  playerXsimp = (playerX+6)/8;
  playerYsimp = (playerY+8)/8;
  windowXmin = playerXsimp - 3;
  windowXmax = playerXsimp + 3;
  windowYmin = playerYsimp - 3;
  windowYmax = playerYsimp + 3;
  if (windowXmin < 0)  windowXmin = 0;
  if (windowXmax > 16) windowXmax = 16;
  if (windowYmin < 0)  windowYmin = 0;
  if (windowYmax > 8)  windowYmax = 8;

  for (x = windowXmin; windowXmax > x; x++)
  {
    for (y = windowYmin; windowYmax > y; y++)
    {
      if (screen[x][y] == 1)
      {
        if (blockType == 2) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, block2, blockMask, 0, 0);
        if (blockType == 3) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, block3, blockMask, 0, 0);
        if (blockType == 4) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, block4, blockMask, 0, 0);
        if (blockType == 5) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, block5, blockMask, 0, 0);
      }
      if (screen[x][y] == 6) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, wood1, blockMask, 0, 0);
      if (screen[x][y] == 7) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, wood2, blockMask, 0, 0);
      if (screen[x][y] == 8) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, wood3, blockMask, 0, 0);
      if (screen[x][y] == 9)
      {
        if (anim6 == 1) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, water1, blockMask, 0, 0);
        if (anim6 == 2) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, water2, blockMask, 0, 0);
        if (anim6 == 3) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, water3, blockMask, 0, 0);
        if (anim6 == 4) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, water4, blockMask, 0, 0);
        if (anim6 == 5) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, water5, blockMask, 0, 0);
        if (anim6 == 6) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, water6, blockMask, 0, 0);
      }
      if (screen[x][y] == 10) Sprites::drawExternalMask((x*8)+xOffset, (y*8)+yOffset, tap1, tap1mask, 0, 0);
    }
  }

  for (x = 0; 16 > x; x++)
  {
    for (y = 0; 8 > y; y++)
    {
      if (anim33 == 15)
      {
        if (screen[x][y] == 10) dripSpawn();
      }
    }
  }
}

void dripDraw()
{
  for (i = 0; dripNumber > i; i++)
  {
    if (dripLife[i] > 0)
    {
      if (dripX[i] > playerX-31 && dripX[i] < playerX+31 &&
          dripY[i] > playerY-31 && dripY[i] < playerY+22)
      {
        if (anim6 < 4) Sprites::drawExternalMask(dripX[i], dripY[i], drip1, drip1mask, 0, 0);
        if (anim6 > 3) Sprites::drawExternalMask(dripX[i], dripY[i], drip2, drip2mask, 0, 0);
      }
    }
  }
}

void cinderDraw()
{
  for (i = 0; cinderNumber > i; i++)
  {
    if (cinderLife[i] > 0 && levelDelay == 0)
    {
      arduboy.drawPixel(cinderX[i]+xOffset, cinderY[i]+yOffset, 1);
    }
  }
}

void flameDraw()
{
  for (i = 0; flameNumber > i; i++)
  {
    if (flameLife[i] > 0)
    {
      if (flameLife[i] > 15)
        Sprites::drawExternalMask(flameX[i]+xOffset, flameY[i]+xOffset, flame2, flame2mask, 0, 0);
      if (flameLife[i] > 4 && flameLife[i] < 15)
        Sprites::drawExternalMask(flameX[i]+xOffset, flameY[i]+yOffset, flame1, flame1mask, 0, 0);
      if (flameLife[i] < 5)
        arduboy.drawPixel(flameX[i]+xOffset, flameY[i]+yOffset, 1);
    }
  }
}

void livesScreen()
{
  if (lives > 0)
  {
    arduboy.clear();
    Sprites::drawOverwrite(0, 31, life1, 0);
    if (lives > 1) Sprites::drawOverwrite(55, 31, life1, 0);
    if (lives > 2) Sprites::drawOverwrite(110, 31, life1, 0);
    arduboy.display();
    arduboy.delayShort(1000);
    arduboy.clear();
    arduboy.display();
  }
}

void winScreen()
{
  arduboy.clear();
  Sprites::drawOverwrite(0, 0, win1, 0);
  arduboy.display();
  arduboy.delayShort(3000);
}

void draw()
{
  drawLevel();
  dripDraw();
  objectsDraw();
  drawMask();
  furnaceDraw();
  drawPlayer();
  flameDraw();
  cinderDraw();
  arduboy.display();
}
