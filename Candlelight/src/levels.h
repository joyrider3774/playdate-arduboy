#pragma once
#include "globals.h"

// Level data arrays (PROGMEM removed, direct indexing used)

const uint8_t level1[] =
{
  1,4,
  1,1,1,1,1,1,
  1,0,0,0,0,0,
  0,0,0,1,1,1,
  0,0,0,0,0,0,
  1,0,0,0,0,1,
  1,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,1,0,0,
  0,0,0,1,0,0,
  0,0,0,1,0,0,
  0,0,0,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,0,0,0,0,
};

const uint8_t level2[] =
{
  2,4,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,1,1,
  0,0,0,0,1,1,
  0,0,0,0,0,0,
  0,0,1,1,1,1,
  0,0,1,1,1,1,
  0,0,0,6,0,0,
  0,0,0,6,0,0,
  0,0,0,6,0,0,
  0,0,0,6,0,0,
  0,0,0,6,0,0,
  0,0,0,6,0,0,
};

const uint8_t level3[] =
{
  3,4,
  0,0,0,0,0,0,
  0,0,0,0,1,0,
  0,0,0,0,0,0,
  0,1,0,0,0,0,
  0,10,0,0,0,0,
  0,1,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  1,1,1,1,1,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,1,1,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
};

const uint8_t level4[] =
{
  4,4,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,0,1,1,1,0,
  0,0,1,1,1,0,
  0,0,1,1,1,0,
  0,0,1,1,1,0,
  0,0,1,1,1,0,
};

const uint8_t level5[] =
{
  5,2,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  10,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,1,1,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
};

const uint8_t level6[] =
{
  6,3,
  0,0,0,1,1,1,
  0,0,0,1,1,1,
  0,0,0,1,1,1,
  0,0,0,1,1,1,
  0,0,0,1,1,1,
  0,0,0,9,1,1,
  0,0,0,9,1,1,
  0,0,0,9,1,1,
  0,0,0,9,1,1,
  0,0,0,1,1,1,
  0,0,1,1,1,1,
  0,1,1,1,1,1,
  0,0,0,0,0,0,
  0,0,1,0,0,0,
};

const uint8_t level7[] =
{
  7,3,
  0,0,0,0,0,6,
  0,0,0,0,0,6,
  0,0,0,0,0,6,
  0,0,6,6,6,6,
  0,0,6,0,0,6,
  0,0,6,0,0,6,
  0,0,6,0,0,6,
  6,6,6,0,0,6,
  0,0,0,0,0,6,
  0,0,0,6,0,6,
  0,0,0,6,0,6,
  0,6,0,0,0,6,
  0,6,0,0,0,6,
  0,6,0,0,0,6,
};

const uint8_t level8[] =
{
  8,3,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  1,1,1,1,1,0,
  10,0,0,0,0,0,
  1,1,1,1,1,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,1,1,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  1,1,1,1,1,0,
  10,0,0,0,0,0,
  1,1,1,1,1,0,
  1,1,1,1,1,0,
};

const uint8_t level9[] =
{
  9,3,
  0,0,0,0,1,0,
  0,0,1,0,1,0,
  0,0,6,0,6,0,
  0,0,6,0,6,0,
  0,0,6,0,6,0,
  0,0,6,0,6,0,
  0,0,1,0,1,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,1,1,1,1,
  0,0,9,1,0,0,
  0,0,1,1,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
};

const uint8_t level10[] =
{
  10,3,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,1,1,1,1,1,
  0,1,1,0,0,0,
  0,1,10,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,1,0,0,
  0,0,0,1,0,0,
  0,0,0,1,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  1,1,1,1,1,1,
  1,1,1,1,1,1,
};

const uint8_t level11[] =
{
  11,4,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  1,1,1,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,1,1,1,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  1,1,1,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
};

const uint8_t level12[] =
{
  12,5,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,1,1,
  0,0,0,0,1,1,
  0,0,0,0,9,1,
  0,0,0,0,9,1,
  0,0,0,0,1,1,
  0,0,1,1,1,1,
  0,0,1,1,1,1,
  0,0,1,1,1,1,
  0,0,1,1,1,1,
};

const uint8_t level13[] =
{
  13,5,
  0,0,0,0,6,0,
  0,0,0,0,6,0,
  0,0,6,0,0,0,
  0,0,6,0,0,0,
  0,0,0,0,0,0,
  0,0,0,1,0,0,
  0,0,0,1,0,0,
  0,0,0,1,0,0,
  1,1,1,1,0,0,
  0,0,6,6,6,6,
  0,0,6,7,6,6,
  0,0,6,6,8,6,
  0,0,6,6,6,6,
  0,0,0,0,0,0,
};

const uint8_t level14[] =
{
  14,5,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,1,1,
  0,0,1,1,1,1,
  0,0,1,1,0,0,
  0,0,9,1,0,0,
  0,0,9,1,0,0,
  0,0,9,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
};

const uint8_t level16[] =
{
  16,2,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,9,1,0,0,
  0,0,9,1,0,0,
  0,0,9,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,0,0,
  0,0,1,1,1,1,
  0,0,0,0,0,9,
  0,0,1,0,0,9,
  0,0,1,0,0,9,
};

const uint8_t level17[] =
{
  17,4,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  10,0,0,0,0,0,
  0,0,0,0,0,0,
  1,1,6,6,1,1,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,0,1,0,0,0,
  0,0,0,1,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
};

// ---- Level loading helpers ----

void levelBorder()
{
  for (x = 0; 16 > x; x++)
  {
    for (y = 0; 8 > y; y++)
    {
      if (x == 0 || y == 0 || x == 16-1 || y == 8-1)
      {
        screen[x][y] = 1;
      }
    }
  }
}

void levelOne()
{
  level = level1[levelRead]; levelRead++;
  blockType = level1[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level1[levelRead]; levelRead++; }
  levelBorder();
  screen[15][6] = 0;
}

void levelTwo()
{
  level = level2[levelRead]; levelRead++;
  blockType = level2[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level2[levelRead]; levelRead++; }
  levelBorder();
  screen[0][6] = 0;
  screen[15][3] = 0;
}

void levelThree()
{
  level = level3[levelRead]; levelRead++;
  blockType = level3[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level3[levelRead]; levelRead++; }
  levelBorder();
  screen[5][7] = 9;
  screen[6][7] = 9;
  screen[0][3] = 0;
  screen[14][7] = 0;
}

void levelFour()
{
  level = level4[levelRead]; levelRead++;
  blockType = level4[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level4[levelRead]; levelRead++; }
  levelBorder();
  screen[11][7] = 9;
  screen[12][7] = 9;
  screen[14][0] = 0;
  screen[0][2] = 0;
  screen[1][7] = 0;
  screen[15][6] = 0;
}

void levelFive()
{
  level = level5[levelRead]; levelRead++;
  blockType = level5[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level5[levelRead]; levelRead++; }
  levelBorder();
  screen[3][7] = 9;
  screen[0][2] = 0;
  screen[15][2] = 0;
}

void levelSix()
{
  level = level6[levelRead]; levelRead++;
  blockType = level6[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level6[levelRead]; levelRead++; }
  levelBorder();
  screen[1][0] = 0;
  screen[15][5] = 0;
}

void levelSeven()
{
  level = level7[levelRead]; levelRead++;
  blockType = level7[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level7[levelRead]; levelRead++; }
  levelBorder();
  for (i = 1; 15 > i; i++) screen[i][7] = 9;
  screen[0][5] = 0;
  screen[15][1] = 0;
}

void levelEaight()
{
  level = level8[levelRead]; levelRead++;
  blockType = level8[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level8[levelRead]; levelRead++; }
  levelBorder();
  screen[4][7] = 9;
  screen[12][7] = 9;
  screen[0][1] = 0;
  screen[15][6] = 0;
}

void levelNine()
{
  level = level9[levelRead]; levelRead++;
  blockType = level9[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level9[levelRead]; levelRead++; }
  levelBorder();
  screen[0][6] = 0;
  screen[11][7] = 0;
}

void levelTen()
{
  level = level10[levelRead]; levelRead++;
  blockType = level10[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level10[levelRead]; levelRead++; }
  levelBorder();
  screen[4][7] = 9;
  screen[5][7] = 9;
  screen[12][0] = 0;
  screen[2][7] = 0;
}

void levelEleven()
{
  level = level11[levelRead]; levelRead++;
  blockType = level11[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level11[levelRead]; levelRead++; }
  levelBorder();
  screen[2][0] = 0;
}

void levelTwelve()
{
  level = level12[levelRead]; levelRead++;
  blockType = level12[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level12[levelRead]; levelRead++; }
  levelBorder();
  screen[3][0] = 10;
  screen[8][0] = 10;
  screen[3][7] = 9;
  screen[4][7] = 9;
  screen[0][6] = 0;
  screen[14][0] = 0;
}

void levelThirteen()
{
  level = level13[levelRead]; levelRead++;
  blockType = level13[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level13[levelRead]; levelRead++; }
  levelBorder();
  screen[14][7] = 0;
  screen[7][0] = 0;
}

void levelFourteen()
{
  level = level14[levelRead]; levelRead++;
  blockType = level14[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level14[levelRead]; levelRead++; }
  levelBorder();
  screen[7][7] = 0;
}

void levelSixteen()
{
  level = level16[levelRead]; levelRead++;
  blockType = level16[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level16[levelRead]; levelRead++; }
  levelBorder();
  screen[15][2] = 0;
  screen[9][7] = 0;
}

void levelSeventeen()
{
  level = level17[levelRead]; levelRead++;
  blockType = level17[levelRead]; levelRead++;
  for (x = 1; 16-1 > x; x++)
    for (y = 1; 8-1 > y; y++)
    { screen[x][y] = level17[levelRead]; levelRead++; }
  levelBorder();
  screen[4][7] = 9;
  screen[9][0] = 0;
  // screen[17][3] = 0;  // original bug: out of bounds for [16][8] grid, removed
}
