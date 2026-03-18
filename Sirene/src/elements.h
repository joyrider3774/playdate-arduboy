#ifndef ELEMENTS_H
#define ELEMENTS_H

#define POWER_UP_HEART             0
#define POWER_UP_SHIELD            1
#define POWER_UP_TRIDENT           2
#define POWER_UP_BUBBLE            3
#define POWER_UP_SEASHELL          4
#define POWER_UP_MAGIC             5
#define POWER_UP_STAR              6

#define SMALL_PILLAR_DEPTH         28
#define BIG_PILLAR_DEPTH           18
#define SUNRAY_OFFSET              32



#include <Arduboy2Common.h>
#include "globals.h"


uint8_t powerUpArrayY[] = {11, 47, 23, 41, 35, 29, 17, 53};
uint8_t bonusCount;

struct Background
{
  public:
    int x;
};

struct Elements
{
  public:
    int x;
    uint8_t y;
    uint8_t type;
    boolean isVisible;
    boolean isActive;
};

struct Bonus
{
  public:
    uint8_t x;
    uint8_t y;
    int amount;
    boolean isVisible;
    boolean isActive;
};

Background Column[3];
Elements powerUP;
Bonus bonus[2];


void setBackground()
{
  backgroundIsVisible = true;
  Column[0].x = 28;
  Column[1].x = 92;
  Column[3].x = 128;
}


void checkBackground()
{
  if (arduboy.everyXFrames(10))
  {
    Column[0].x--;
    Column[1].x--;
  }
  if (arduboy.everyXFrames(3)) Column[2].x--;

  if (Column[0].x < -8) Column[0].x = 128;
  if (Column[1].x < -8) Column[1].x = 128;
  if (Column[2].x < -16) Column[2].x = 128;
}

void checkPowerUP()
{
  if (powerUP.isActive)
  {
    if (arduboy.everyXFrames(5)) powerUP.isVisible = !powerUP.isVisible;
    if (arduboy.everyXFrames(2)) powerUP.x--;
    if (powerUP.x < -8) powerUP.isActive = false;
  }
}


void checkBonus()
{
  for (uint8_t i = 0; i < 2; i++)
  {
    if (bonus[i].isActive)
    {
      bonus[i].y--;
      bonus[i].isVisible = !bonus[i].isVisible;
      if (bonus[i].y < 2)
      {
        bonus[i].isActive = false;
        bonus[i].isVisible = false;
      }
    }
  }
}


void giveBonus(int amountBonus, uint8_t positionX, uint8_t positionY)
{
  bonusCount++;
  if (bonusCount > 1) bonusCount = 0;
  bonus[bonusCount].x = positionX;
  bonus[bonusCount].y = positionY;
  bonus[bonusCount].amount = amountBonus;
  scorePlayer += amountBonus,
                 bonus[bonusCount].isActive = true;
  bonus[bonusCount].isVisible = true;
}


void powerUPSet(uint8_t typeSet)
{
  powerUP.type = typeSet;
  powerUP.x = 128;
  powerUP.y = powerUpArrayY[powerUpSelectorY];
  powerUP.isActive = true;
  powerUpSelectorY = (++powerUpSelectorY) % 8;
}

void drawBackground()
{
  if (backgroundIsVisible)
  {
    for (uint8_t z = 0; z < 2; z++)
    {
      for (uint8_t i = 0; i < 3; i++)
      {
        sprites.drawSelfMasked(SUNRAY_OFFSET + (z * 15) + (8 * i), (8 * i), sunRay, i);
      }
    }
    sprites.drawPlusMask(Column[0].x, SMALL_PILLAR_DEPTH, columnSmall_plus_mask, 0);
    sprites.drawPlusMask(Column[1].x, SMALL_PILLAR_DEPTH, columnSmall_plus_mask, 0);
    sprites.drawPlusMask(Column[2].x, BIG_PILLAR_DEPTH, columnBig_plus_mask, 0);
    sprites.drawPlusMask(Column[2].x + 8, BIG_PILLAR_DEPTH, columnSmall_plus_mask, 0);
  }
}


void drawPowerUP()
{
  if (powerUP.isActive && powerUP.isVisible) sprites.drawPlusMask(powerUP.x, powerUP.y, powerUP_plus_mask, powerUP.type);
}



void drawBonus()
{
  for (uint8_t i = 0; i < 2; i++)
  {
    if (bonus[i].isActive && bonus[i].isVisible)
    {
      char buf[10];
      ltoa(bonus[i].amount, buf, 10);
      char charLen = strlen(buf);

      sprites.drawSelfMasked(bonus[i].x, bonus[i].y, numbersSmall, 10);

      for (uint8_t k = 0; k < charLen; k++)
      {
        char digit = buf[k];
        uint8_t j;
        if (digit <= 48)
        {
          digit = 0;
        }
        else {
          digit -= 48;
          if (digit > 9) digit = 0;
        }

        for (uint8_t z = 0; z < 10; z++)
        {
          if (digit == z) j = z;
        }
        sprites.drawSelfMasked(bonus[i].x + 4 + (5 * k), bonus[i].y, numbersSmall, digit);
      }
    }
  }
}


#endif
