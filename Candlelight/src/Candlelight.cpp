// Candlelight
// Version 1.00
// April 30th 24 - May 10th 24
// Matthew Bryan
// Ported to Playdate by Arduboy2Playdate

#include <Arduboy2.h>
#include <ArduboyTones.h>

// Library instances (definitions)
Arduboy2Base arduboy;
Sprites sprites;
ArduboyTones sound(arduboy.audio.enabled);

// Variable definitions
bool firstRun = 1;
bool start = 0;
bool s = 1;
int i;
int c;
int direction = 1;
int r;
int gameProgress = 0;
int introStep = 0;
int introCounter = 0;
int anim2;
int anim3;
int anim4;
int anim6;
int anim33;
int levelDelay;
double gravity = 0.37;
uint8_t levelRead;
int level;
int newLevel;
int newX;
int newY;
bool furnaceLit1;
bool furnaceLit2;
bool furnaceLit3;
int winDelay;

int playerX = 0;
int playerY = 0;
int playerXvel;
double playerYvel;
uint8_t playerJump;
uint8_t playerDash;
bool jumpPressed;
bool dashPressed;
uint8_t touchdown;
uint8_t playerDeath;
int playerXsimp;
int playerYsimp;
bool headBump;
bool leftWall;
bool rightWall;
bool land = 1;
int fall;
uint8_t moveRight;
uint8_t moveLeft;
uint8_t yHeight = 5;
uint8_t lives = 3;

uint8_t screen[16][8];
uint8_t x = 0;
uint8_t y = 0;
int xOffset = 0;
int yOffset = 0;
int windowXmin;
int windowXmax;
int windowYmin;
int windowYmax;
int blockType;

uint8_t flameNumber = 10;
uint8_t flameLife[11];
double flameX[11];
double flameY[11];

uint8_t cinderNumber = 30;
uint8_t cinderLife[31];
int cinderX[31];
int cinderY[31];

uint8_t dripNumber = 4;
uint8_t dripLife[5];
int dripX[5];
int dripY[5];

#include "images.h"
#include "sounds.h"
#include "globals.h"
#include "levels.h"
#include "draw.h"
#include "player.h"
#include "game.h"

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(50);
  arduboy.audio.begin();
  sound.begin();
  // Skip Arduboy dissolve animation — not applicable on Playdate
}

void loop()
{
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();

  if (!start)
  {
    // ---- Intro state machine (replaces blocking firstRun loops) ----
    if (firstRun)
    {
      // Step 0: Slide "Onebit" logo down (30 frames, playerY: -10 -> 20)
      if (introStep == 0)
      {
        if (introCounter == 0) playerY = -10;
        Sprites::drawOverwrite(20, playerY, Onebit, 0);
        playerY++;
        introCounter++;
        if (introCounter >= 30) { introStep = 1; introCounter = 0; }
      }
      // Step 1: Slide "Productions" in from right (79 frames, playerX: 128 -> 49)
      else if (introStep == 1)
      {
        if (introCounter == 0) playerX = 128;
        Sprites::drawOverwrite(20, 20, Onebit, 0);
        Sprites::drawOverwrite(playerX, 20, Productions, 0);
        playerX--;
        introCounter++;
        if (introCounter >= 79) { introStep = 2; introCounter = 0; }
      }
      // Step 2: Slide "Presents" up (36 frames, playerY: 65 -> 29)
      else if (introStep == 2)
      {
        if (introCounter == 0) playerY = 65;
        Sprites::drawOverwrite(20, 20, Onebit, 0);
        Sprites::drawOverwrite(49, 20, Productions, 0);
        Sprites::drawOverwrite(49, playerY, Presents, 0);
        playerY--;
        introCounter++;
        if (introCounter >= 36) { introStep = 3; introCounter = 0; }
      }
      // Step 3: Hold "Onebit Productions Presents" (~75 frames = 1500ms at 50fps)
      else if (introStep == 3)
      {
        Sprites::drawOverwrite(20, 20, Onebit, 0);
        Sprites::drawOverwrite(49, 20, Productions, 0);
        Sprites::drawOverwrite(49, 29, Presents, 0);
        introCounter++;
        if (introCounter >= 75) { introStep = 4; introCounter = 0; }
      }
      // Step 4: Show Candlelight title + life icon (~125 frames = 2500ms at 50fps)
      else if (introStep == 4)
      {
        Sprites::drawOverwrite(0, 0, Candlelight_img, 0);
        Sprites::drawOverwrite(31, 31, life1, 0);
        introCounter++;
        if (introCounter >= 125) { introStep = 5; introCounter = 0; }
      }
      // Step 5: Intro done
      else
      {
        firstRun = 0;
        playerX = 0;
        introStep = 0;
        introCounter = 0;
      }
      arduboy.display();
      return;
    }

    // ---- Title screen ----
    if (direction == 1) playerX++;
    if (direction == 2) playerX--;
    if (playerX > 128)  direction = 2;
    if (playerX < -114) direction = 1;

    Sprites::drawOverwrite(playerX, 43, startGame, 0);
    Sprites::drawOverwrite(89, 9, speaker1, 0);
    if (s == 1) Sprites::drawOverwrite(108, 9, sound1, 0);

    arduboy.display();

    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
    {
      start = 1;
      newGame();
    }
    if (arduboy.pressed(RIGHT_BUTTON)) s = 1;
    if (arduboy.pressed(LEFT_BUTTON))  s = 0;

    return;
  }

  // ---- Main game loop ----
  animations();
  gameUnfold();
  draw();
}
