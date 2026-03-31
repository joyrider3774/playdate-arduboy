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
#include "config.h"
#include "player.h"
#include "utils.h"

// Delay times in millis
const unsigned int SHOW_LEVEL_NUMBER_TIME  = 1000;
const unsigned int SHOW_LEVEL_TIME  = 1000;
const unsigned int SHOW_LEVEL_COMPLETE_TIME = 1500;
const unsigned int SHOW_CONGRATULATIONS_TIME = 2000;
const unsigned int SHOW_GAME_COMPLETE_TIME = 3000;
const unsigned int SHOW_PLAYER_DEAD_TIME = 600;
const unsigned int SHOW_GAME_OVER_TIME = 2000;


// Gift (show -> blink -> disappear)
const unsigned int  GIFT_STARTS_BLINKING_TIME = 2500;
const unsigned int  GIFT_DISAPPEARS_TIME = 4000;
const unsigned int  GIFT_BLINKING_TIME = 200;

const unsigned int  SHIELD_DISAPPEARS_TIME = 2000;



#define STATE_SHOW_LEVEL_NUMBER       0
#define STATE_SHOW_LEVEL              1
#define STATE_PLAYING                 2

int gameState;

#define MODIFIED_SPEED_NONE        0
#define MODIFIED_SPEED_SLOW        1
#define MODIFIED_SPEED_FAST        2

#define DIFFICULTY_VERY_EASY        0
#define DIFFICULTY_EASY             1
#define DIFFICULTY_NORMAL           2
#define DIFFICULTY_HARD             3
#define DIFFICULTY_VERY_HARD        4
#define DIFFICULTY_ULTRA            5

const char PROGMEM STRING_LEVEL_COMPLETED[] = "LEVEL COMPLETED";
const char PROGMEM STRING_GAME_OVER[] = "GAME OVER";
const char PROGMEM STRING_CONGRATULATIONS[] = "CONGRATULATIONS";
const char PROGMEM STRING_GAME_COMPLETE[] = "GAME COMPLETED";
const char PROGMEM STRING_DEMO_COMPLETE[] = "DEMO COMPLETED";

const char PROGMEM STRING_LEVEL[] = "LEVEL ";
const char PROGMEM STRING_X[] = "x";
const char PROGMEM STRING_FPS[] = "FPS ";

// Only for the balls
unsigned char modifiedSpeed;


//GAME
unsigned int lives;
unsigned char level;
unsigned int difficulty;
const int PLAYER_RAY_LAUNCH_FRAME_TIME = 300;

//PLAYER
int playerPosX;
int playerPosY;
int playerSpeed;
boolean isPlayerAlive;

//RAYS
unsigned char raySpeed;
int rayPosX[MAX_RAYS];
int rayPosY[MAX_RAYS];
boolean isRayVisible[MAX_RAYS];
boolean isRayJustLaunched;
long rayLaunchedTime;

//BALLS
unsigned char balls[MAX_BALLS];
int ballsPosX[MAX_BALLS];
int ballsPosY[MAX_BALLS];
int ballsSpeedX[MAX_BALLS];
int ballsSpeedY[MAX_BALLS];
unsigned char ballsGift[MAX_BALLS];
int ballFrame;
const int NUMBER_OF_BALL_FRAMES = 4;
boolean hasLuckyGifts;

//GIFT
boolean isGiftVisible = false;
unsigned char giftType;
int giftPosX;
int giftPosY;
unsigned int giftSpeed;
int giftTime;
unsigned char numItems;
int shieldTime;

//BACKGROUND
unsigned char backgroundType;
boolean isBackgroundMirror;
boolean hasBackgroundNoise;

//DEBUG
unsigned long previousTime;
unsigned char fps;
unsigned int counter = 0;

//FEATURES
boolean isDoubleRayEnabled;
boolean isShieldEnabled;
boolean isNoGravityEnabled;
boolean isNegativeEnabled;
boolean isMirrorEnabled;
boolean areBallsRotating;


//FUNCTIONS

void resetGame();

static void updateBallsPosition();
static void updatePlayerPosition();
static void updateGiftPosition();
static void updateRaysPosition();

static boolean checkPlayerCollision();
static void checkGiftCollision();
static void checkRaysCollision();

static void drawBanner (const char* text);
static void drawLevelBanner (unsigned char level);
static void drawCenteredBanner (uint8_t lenght);
static void drawBackground();
static void drawBalls();
static void drawRays();
static void drawGift();
static void drawPlayer();
static void drawItems();
static void drawLives();

static boolean checkLevelComplete();
static int checkFirstInvisibleBall();

void initBalls();
static char getLuckyGift();
static void loadLevel(const unsigned char* levelArray);
extern void setMenuMode();
extern void setLogoMode();
void customDelay(int time);
void loseShield();




void playGame() {

  // Return to menu if player press key A (pause) , but keep all the game states
  if (helper.justPressed(A_BUTTON)) {
    if (isNegativeEnabled) arduboy.invert(false);
    setMenuMode();
  }

  isPlayerAlive = true;


  // Update position of all elements

  if (gameState == STATE_PLAYING) {
    updateBallsPosition();
    updatePlayerPosition();
    updateRaysPosition();
    updateGiftPosition();
  }

  // Check collision
  checkGiftCollision();
  checkRaysCollision();
  // Check also if player got the death gift
  isPlayerAlive = isPlayerAlive ? !checkPlayerCollision() : false;

#if defined GOD_MODE || defined PERFORMANCE_TEST
  isPlayerAlive = true;
#endif


  //Draw elements
  drawBackground();
  drawBalls();
  drawRays();
  drawGift();
  drawPlayer();

  // Noise affects previous elements
  if (hasBackgroundNoise && gameState == STATE_PLAYING) {
    helper.addNoise(NOISE_LEVEL);
  }

  //Draw borders
  arduboy.drawRect(0, 0, WIDTH, HEIGHT, WHITE);


  //Items and lives not mirrored
  if (isMirrorEnabled) {
    helper.mirrorScreen();
  }

  if (gameState == STATE_SHOW_LEVEL_NUMBER) {
    drawLevelBanner (level + 1);
    previousTime = 0;
  }

  drawItems();

  //Draw in-game fps
  if (showFPS) {

    //if (gameState == STATE_PLAYING && arduboy.everyXFrames(30)){
    if (gameState == STATE_PLAYING && ((counter++ % 30) == 0)) {
      long actualTime = millis();
      if (previousTime != 0) {
        fps = (30 * 1000 / (actualTime - previousTime));
      }
      previousTime = actualTime;
    }
    //arduboy.setCursor(6, 3);
    //arduboy.print(F("FPS: "));
    helper.print( STRING_FPS, 6, 3);
    arduboy.print(fps);
  }

  // Check if player lost a life
  if (!isPlayerAlive && !checkLevelComplete()) {

    if (sound) {
#ifdef ARDUBOY_2
      tunes->tone(750, 200);
#else
      arduboy.tunes.tone(750, 200);
#endif
    }
    lives--;
    if (lives == 0 && immortal) lives = 1;
  }

  drawLives();


  //End draw

  if (checkLevelComplete()) {
    level++;
    boolean gameCompleted = (sizeof(LEVELS) / sizeof(LEVELS[0])) <= level;

    if (gameCompleted) {
      drawBanner(STRING_CONGRATULATIONS);
      helper.display();
      //TODO Do with states better than delays
      customDelay(SHOW_CONGRATULATIONS_TIME);

#ifdef DEMO
      drawBanner(STRING_DEMO_COMPLETE);
#else
      drawBanner(STRING_GAME_COMPLETE);
#endif

      helper.display();
      customDelay(SHOW_GAME_COMPLETE_TIME);

      // Reset level
      if (autosave) {
        EEPROM.write(EEPROM_LAST_LEVEL, 0);
      }

      if (isNegativeEnabled) arduboy.invert(false);
      setLogoMode();
      game = GAME_STOP;

    } else {
      // Level complete
      drawBanner(STRING_LEVEL_COMPLETED);
      helper.display();
      customDelay(SHOW_LEVEL_COMPLETE_TIME);

      if (autosave) {
        EEPROM.write(EEPROM_LAST_LEVEL, level);
        EEPROM.write(EEPROM_LAST_LIVES, lives);
      }

      initBalls();
    }

    return;
  }

  // Check if player is alive
  if (!isPlayerAlive) {
    // Player has no more lives
    if (lives == 0) {
      drawBanner(STRING_GAME_OVER);
      helper.display();
      customDelay(SHOW_GAME_OVER_TIME);

      if (autosave) {
        EEPROM.write(EEPROM_LAST_LEVEL, 0);
      }

      // Back to menu
      if (isNegativeEnabled) arduboy.invert(false);
      setLogoMode();
      game = GAME_STOP;
    } else {
      //Player has more lives
      helper.display();
      customDelay(SHOW_PLAYER_DEAD_TIME);
      initBalls();
    }
    return;
  }

  helper.display();

  if (gameState == STATE_SHOW_LEVEL_NUMBER) {

    customDelay(SHOW_LEVEL_NUMBER_TIME);
    gameState = STATE_SHOW_LEVEL;

  } else  if (gameState == STATE_SHOW_LEVEL) {
    customDelay(SHOW_LEVEL_TIME);

    if (hasBackgroundNoise) {
      //DANGER MODE

      const int FREQ_TONE = 500;
      const int BEEP_TIME = 500;
      const int NOISE = 2;
      int i;
      for (i = 0; i < 6; i++) {
        arduboy.clear();
        arduboy.setRGBled(((i + 1) % 2 == 0) ? 255 : 0, 0 , (i % 2 == 0) ? 255 : 0);

        if (i % 2 == 0) {
          if (sound) {
#ifdef ARDUBOY_2
            tunes->tone(FREQ_TONE, BEEP_TIME);
#else
            arduboy.tunes.tone(FREQ_TONE, BEEP_TIME);
#endif
          }

          helper.drawMaskedBitmap((WIDTH - DANGER_SIZE) / 2, (HEIGHT - DANGER_SIZE) / 2,
                                  DANGER_BITMAP,
                                  DANGER_BITMAP,
                                  DANGER_SIZE, DANGER_SIZE, 0);
          helper.display();
          delay(BEEP_TIME);
        } else {
          for (int j = 0; j < 20; j++) {
            arduboy.clear();
            helper.addNoise(NOISE);
            helper.display();
            customDelay(BEEP_TIME / 20);
          }
        }
      }
      arduboy.setRGBled(0, 0, 0);
      /*  arduboy.clear();
        helper.addNoise(NOISE);
        helper.display();
        customDelay(BEEP_TIME);  */
    }
    // Start playing
    gameState = STATE_PLAYING;
  }
}



//END DRAW


//UPDATE POSITIONS


static void updateBallsPosition() {

  int loopTimes = frameDrop;

  //TODO check if it's possible to do better simulating more fps -> more loop
  if (difficulty == DIFFICULTY_EASY && arduboy.everyXFrames(isNoGravityEnabled ? 5 : 4)) return;
  if (difficulty == DIFFICULTY_VERY_EASY && arduboy.everyXFrames(isNoGravityEnabled ? 4 : 3)) return;
  if (difficulty == DIFFICULTY_ULTRA) loopTimes *= 3;
  if (difficulty == DIFFICULTY_VERY_HARD) loopTimes *= 2;
  if (difficulty == DIFFICULTY_HARD && arduboy.everyXFrames(2)) loopTimes *= 2;

  if (modifiedSpeed == MODIFIED_SPEED_FAST) {
    loopTimes *= 2;
  } else if (modifiedSpeed == MODIFIED_SPEED_SLOW) {
    loopTimes /= 2;
    //fix for frameDrop = 1;
    if (loopTimes == 0) {
      loopTimes = 1;
      if (arduboy.everyXFrames(2)) {
        return;
      }
    }
  }

  int i, j;
  for (j = 0; j < loopTimes ; j++) {
    for (i = 0; i < MAX_BALLS; i++) {

      if (balls[i] == BALL_INVISIBLE) continue;

      //move X
      ballsPosX[i] += ballsSpeedX[i];

      //check limits
      if (ballsPosX[i] < 0) {
        ballsPosX[i] = 0;
        ballsSpeedX[i] = - ballsSpeedX[i];
      } else if (ballsPosX[i] / SCREEN_MAGNIFICATION + BALL_SIZES[balls[i]] > WIDTH) {
        ballsPosX[i] = (WIDTH - BALL_SIZES[balls[i]]) * SCREEN_MAGNIFICATION;
        ballsSpeedX[i] = - ballsSpeedX[i];
      }

      //move Y
      ballsSpeedY[i] += isNoGravityEnabled ? 0 : GRAVITY_ACCEL;
      ballsPosY[i] +=  ballsSpeedY[i];

      //check limits
      if (ballsPosY[i] < 0) {
        ballsPosY[i] = 0;
        ballsSpeedY[i] = isNoGravityEnabled ? - ballsSpeedY[i] :  0;
      } else if (ballsPosY[i] / SCREEN_MAGNIFICATION + BALL_SIZES[balls[i]] > HEIGHT) {
        ballsPosY[i] = (HEIGHT - BALL_SIZES[balls[i]]) * SCREEN_MAGNIFICATION;
        ballsSpeedY[i] =  isNoGravityEnabled ? -ballsSpeedY[i] : - BALL_Y_SPEEDS[balls[i]];
      }
    }
  }
}


static void updatePlayerPosition() {

  //move
  if (arduboy.pressed(LEFT_BUTTON))  playerPosX -= playerSpeed;
  if (arduboy.pressed(RIGHT_BUTTON)) playerPosX += playerSpeed;

  //check limits
  if (playerPosX < 0) {
    playerPosX = 0;
  } else if (playerPosX / SCREEN_MAGNIFICATION + PLAYER_WIDTH > WIDTH) {
    playerPosX = (WIDTH - PLAYER_WIDTH) * SCREEN_MAGNIFICATION;
  }
}


static void updateRaysPosition() {

  isRayJustLaunched = false;
  int i;
  for (i = 0; i < MAX_RAYS; i++) {

    if (isRayVisible[i]) {
      rayPosY[i] -= raySpeed;
      if (rayPosY[i] < 0) {
        isRayVisible[i] = false;
      }
    } else if (gameState == STATE_PLAYING && helper.justPressed(B_BUTTON | UP_BUTTON ) && !isRayJustLaunched) {

      rayPosX[i] = playerPosX + PLAYER_WIDTH * SCREEN_MAGNIFICATION / 2 - RAY_WIDTH * SCREEN_MAGNIFICATION / 2;
      rayPosY[i] = (HEIGHT - PLAYER_HEIGHT) * SCREEN_MAGNIFICATION;
      isRayVisible[i] = true;
      isRayJustLaunched = true;
      rayLaunchedTime = millis();
    }

    if (!isDoubleRayEnabled) break;
  }
}


static void updateGiftPosition() {

  if (isGiftVisible) {
    giftPosY += giftSpeed;
    if (giftPosY > (HEIGHT - 1 - GIFT_SIZE) * SCREEN_MAGNIFICATION) giftPosY = (HEIGHT - 1 - GIFT_SIZE) * SCREEN_MAGNIFICATION;

    giftTime += (1000 * frameDrop) / 60;
    if (giftTime >= GIFT_DISAPPEARS_TIME) isGiftVisible = false;
  }

  if (!isShieldEnabled && shieldTime > 0){
    shieldTime -= (1000 * frameDrop) / 60;
    if (shieldTime < 0) shieldTime = 0;   
  }
}

//END UPDATE POSITIONS


//COLLISIONS


static void checkGiftCollision() {
  if (!isGiftVisible)return;

  if (!( playerPosX  > giftPosX + GIFT_SIZE * SCREEN_MAGNIFICATION
         || playerPosX  + PLAYER_WIDTH  * SCREEN_MAGNIFICATION < giftPosX
         || playerPosY  > giftPosY + GIFT_SIZE * SCREEN_MAGNIFICATION
         || playerPosY  + PLAYER_HEIGHT * SCREEN_MAGNIFICATION < giftPosY
       )) {
    // get the gift

    if (giftType == GIFT_LIFE) {
      lives++;
    } else if (giftType == GIFT_DOUBLE_RAY) {
      isDoubleRayEnabled = true;
    } else if (giftType == GIFT_SHIELD) {
      isShieldEnabled = true;
      shieldTime = 0;
    } else if (giftType == GIFT_MIRROR) {
      isMirrorEnabled = !isMirrorEnabled;
    } else if (giftType == GIFT_NEGATIVE) {
      isNegativeEnabled = !isNegativeEnabled;
      arduboy.invert(isNegativeEnabled);
    } else if (giftType == GIFT_DEATH) {
      if (isShieldEnabled || shieldTime != 0){
        loseShield();  
      } else isPlayerAlive = false;
    } else if (giftType == GIFT_SLOW) {
      modifiedSpeed = MODIFIED_SPEED_SLOW;
    } else if (giftType == GIFT_FAST) {
      modifiedSpeed = MODIFIED_SPEED_FAST;
    }
    // Do no delete gift if GIFT_DEATH
    if (isPlayerAlive) isGiftVisible = false;
  }
}


static void checkRaysCollision() {

  int i, j;
  for (j = 0; j < MAX_RAYS; j++) {

    if (!isRayVisible[j]) continue;
    for (i = 0; i < MAX_BALLS; i++) {

      if (balls[i] == BALL_INVISIBLE) continue;


      if (!( rayPosX[j] > ballsPosX[i] + BALL_SIZES[balls[i]] * SCREEN_MAGNIFICATION
             || rayPosX[j] + RAY_WIDTH * SCREEN_MAGNIFICATION < ballsPosX[i]
             || rayPosY[j] > ballsPosY[i] + BALL_SIZES[balls[i]] * SCREEN_MAGNIFICATION
             || rayPosY[j] + RAY_HEIGHT * SCREEN_MAGNIFICATION < ballsPosY[i]
           )) {

        //Ray touches a ball
        if (sound) {
#ifdef ARDUBOY_2
          tunes->tone(175, 100);
#else
          arduboy.tunes.tone(175, 100);
#endif
        }

        isRayVisible[j] = false;

        int newBall = checkFirstInvisibleBall();

        // Launch gift only if no other is visible
        if (!isGiftVisible) {

          // Get lucky/random gift if the ball has no gift
          if (!ballsGift[i] && hasLuckyGifts) ballsGift[i] = getLuckyGift();


          if (ballsGift[i]) {
            giftType = ballsGift[i];
            //Better on ray position
            //giftPosX = ballsPosX[i] + (BALL_SIZES[balls[i]] * SCREEN_MAGNIFICATION) / 2;
            giftPosX = rayPosX[j] - ((GIFT_SIZE - RAY_WIDTH) * SCREEN_MAGNIFICATION) / 2;
            //giftPosY = ballsPosY[i] + (BALL_SIZES[balls[i]] * SCREEN_MAGNIFICATION) / 2;
            //From top of the ball to see better the gift
            giftPosY = ballsPosY[i];
            //check off screen
            if (giftPosY > (HEIGHT - GIFT_SIZE) * SCREEN_MAGNIFICATION) giftPosY = (HEIGHT - GIFT_SIZE) * SCREEN_MAGNIFICATION;
            isGiftVisible = true;
            giftTime = 0;
            //Remove gift of the ball
            ballsGift[i] = 0;
          }
        }

        //Make ball smaller
        balls[i]--;

        //Only if not invisible
        if (balls[i] != BALL_INVISIBLE) {

          //Modify ball
          ballsSpeedX[i] = BALL_X_SPEEDS[balls[i]];
          ballsSpeedY[i] = isNoGravityEnabled ? - BALL_X_SPEEDS[balls[i]] : - (BALL_Y_SPEEDS[balls[i]] / 2);
          //ballsSize[i] = BALL_SIZES[balls[i]];

          //33% faster
          if (isNoGravityEnabled) {
            ballsSpeedX[i] = (ballsSpeedX[i] * 4 ) / 3;
            ballsSpeedY[i] = (ballsSpeedY[i] * 4 ) / 3;
          }

          //Duplicate ball
          balls[newBall] = balls[i];
          ballsPosX[newBall] = ballsPosX[i];
          ballsPosY[newBall] = ballsPosY[i];
          ballsSpeedX[newBall] = - ballsSpeedX[i];
          ballsSpeedY[newBall] = ballsSpeedY[i];

        }

        // TODO check if better return;
        break;
      }
    }
  }
}



static boolean checkPlayerCollision() {

  int i;

  if (shieldTime != 0) return false;
  
  // TODO Check better
  int HIT_BOX_PADDING = 2;
  int HIT_BOX_PADDING_MAGNIFICATION = HIT_BOX_PADDING * SCREEN_MAGNIFICATION;

  for (i = 0; i < MAX_BALLS; i++) {

    if (balls[i] == BALL_INVISIBLE) continue;

    if (!( playerPosX + HIT_BOX_PADDING_MAGNIFICATION > ballsPosX[i] + BALL_SIZES[balls[i]] * SCREEN_MAGNIFICATION
           || playerPosX + HIT_BOX_PADDING_MAGNIFICATION + (PLAYER_WIDTH - HIT_BOX_PADDING * 2) * SCREEN_MAGNIFICATION < ballsPosX[i]
           || playerPosY + HIT_BOX_PADDING_MAGNIFICATION > ballsPosY[i] + BALL_SIZES[balls[i]] * SCREEN_MAGNIFICATION
           || playerPosY + HIT_BOX_PADDING_MAGNIFICATION + (PLAYER_HEIGHT - HIT_BOX_PADDING * 2) * SCREEN_MAGNIFICATION < ballsPosY[i]
         )) {
      if (isShieldEnabled){
        loseShield();
        return false;
        
      } else return true;
    }
  }
  return false;
}


//END COLLISIONS


void initBalls() {

  memset(balls, BALL_INVISIBLE, sizeof balls);

  gameState = STATE_SHOW_LEVEL_NUMBER;

  int i;
  for (i = 0; i < MAX_RAYS; i++) {
    isRayVisible[i] = false;
  }
  ballFrame = 0;
  isGiftVisible = false;
  shieldTime = 0;

  //TODO Check if this is needed or they are set after the level load
  /*
    isDoubleRayEnabled = false;
    isNoGravityEnabled = false;
    isNegativeEnabled = false;
    isMirrorEnabled = false;
    areBallsRotating = false;
    modifiedSpeed = MODIFIED_SPEED_NONE;
  */

  // X player position defined dinamically later
  playerPosY = (HEIGHT - PLAYER_HEIGHT) * SCREEN_MAGNIFICATION;
  //Removed, now is not possible
  //if ((sizeof(LEVELS) / sizeof(LEVELS[0])) <= level) level = 0;
  loadLevel(LEVELS[level]);

}


static void loadLevel(const unsigned char* levelArray) {

  unsigned char ballsNumber = pgm_read_byte(levelArray) & B00001111;
  isBackgroundMirror = pgm_read_byte(levelArray) & BACKGROUND_MIRROR;
  hasBackgroundNoise = pgm_read_byte(levelArray) & BACKGROUND_NOISE;
  backgroundType = (pgm_read_byte(levelArray) & B00110000);
  isNoGravityEnabled = backgroundType == BACKGROUND_MOON || backgroundType == BACKGROUND_EMPTY_MOON ? true : false;

  //areBallsRotating = background == BACKGROUND_MOON ? true : false;
  char gifts = pgm_read_byte(levelArray + 1);
  isDoubleRayEnabled = gifts & GIFT_DOUBLE_RAY;
  isShieldEnabled = gifts & GIFT_SHIELD;
  isNegativeEnabled = gifts & GIFT_NEGATIVE;
  arduboy.invert(isNegativeEnabled);
  isMirrorEnabled = gifts & GIFT_MIRROR;
  modifiedSpeed = (gifts & B11000000) == 0 ? MODIFIED_SPEED_NONE : ((gifts & B11000000) == GIFT_FAST  ? MODIFIED_SPEED_FAST : MODIFIED_SPEED_SLOW);

  hasLuckyGifts = !(gifts & NO_LUCKY_GIFTS);

  playerPosX = ((pgm_read_byte(levelArray + 2) & B01111111) - (PLAYER_WIDTH / 2)) * SCREEN_MAGNIFICATION;
  areBallsRotating = (pgm_read_byte(levelArray + 2) & B10000000);

  unsigned char i;

  for (i = 0; i < ballsNumber; i++) {

    balls[i] = pgm_read_byte(levelArray + i * 3 + 3) & B00000011;
    ballsGift[i] = pgm_read_byte(levelArray + i * 3 + 3) & B11111100;
    // Relative to center of the ball
    ballsPosX[i] = ((pgm_read_byte(levelArray + i * 3 + 4) & B01111111) - BALL_SIZES[balls[i]] / 2) * SCREEN_MAGNIFICATION;
    ballsPosY[i] = ((pgm_read_byte(levelArray + i * 3 + 5) & B01111111) - BALL_SIZES[balls[i]] / 2) * SCREEN_MAGNIFICATION;
    ballsSpeedX[i] = (pgm_read_byte(levelArray + i * 3 + 4) & INVERTED_MOTION) ? - BALL_X_SPEEDS[balls[i]] : BALL_X_SPEEDS[balls[i]];
    ballsSpeedY[i] = isNoGravityEnabled ? (pgm_read_byte(levelArray + i * 3 + 5) & INVERTED_MOTION) ? - BALL_X_SPEEDS[balls[i]] :  BALL_X_SPEEDS[balls[i]] : 0;

    //33% faster
    if (isNoGravityEnabled) {
      ballsSpeedX[i] = (ballsSpeedX[i] * 4 ) / 3;
      ballsSpeedY[i] = (ballsSpeedY[i] * 4 ) / 3;
    }
  }
}



static char getLuckyGift() {

  if (rand() % LUCKY_GIFTS_RATIO == 0) {
    //rand()%N better size that random(N)

    // if/else 8 bytes less that switch
    uint8_t gift = rand() % 6;
    if (gift == 0) {
      return GIFT_LIFE;
    } else if (gift == 1) {
      return GIFT_DOUBLE_RAY;
    } else if (gift == 2) {
      return GIFT_NEGATIVE;
    } else if (gift == 3) {
      return GIFT_MIRROR;
    } else if (gift == 4) {
      return GIFT_SLOW;
    } else if (gift == 5){
      return GIFT_SHIELD;
    }
  }
  return 0;
}


// Hack to display on delays with SCREEN_CAST
void customDelay(int time) {

#ifdef SCREEN_CAST

  int i;
  int times = time / 50;

  for (i = 0; i < times; i++) {
    delay(50);
    helper.display();
  }

#else
  delay(time);
#endif
}

static boolean checkLevelComplete() {

  int i;
  for (i = 0; i < MAX_BALLS; i++) {
    if (balls[i] != BALL_INVISIBLE) return false;
  }

  return true;
}


static int checkFirstInvisibleBall() {

  int i;
  for (i = 0; i < MAX_BALLS; i++) {
    if (balls[i] == BALL_INVISIBLE) return i;
  }
  return 0;
}



void resetGame() {
  level = 0;
  lives = INITIAL_LIVES;
  initBalls();
}

void configGame() {

  //After a new game or continue, maybe we changed fps settings -> readjust
  game = GAME_RUNNING;
  arduboy.setFrameRate(60 / frameDrop);

  //To set/clear negative screen
  arduboy.invert(isNegativeEnabled);

  playerSpeed = PLAYER_SPEED * frameDrop;
  raySpeed = RAY_SPEED * frameDrop;
  giftSpeed = GIFT_SPEED * frameDrop;

  previousTime  = 0;
  fps = 0;

}


// DRAW

static void drawBackground() {

  const unsigned char BACKGROUND_TOP = 12;
  helper.drawBitmap((WIDTH - BACKGROUND_WIDTH) / 2, BACKGROUND_TOP, backgroundType == BACKGROUND_MOUNTAIN ? BACKGROUND_MOUNTAIN_BITMAP : backgroundType == BACKGROUND_CITY ? BACKGROUND_CITY_BITMAP : backgroundType == BACKGROUND_MOON ?  BACKGROUND_MOON_BITMAP : BACKGROUND_MOON_BITMAP, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, isBackgroundMirror);

  //draw extra background if needed
  if (backgroundType == BACKGROUND_MOON) {
    helper.drawBitmap((WIDTH - BACKGROUND_WIDTH) / 2, BACKGROUND_TOP + BACKGROUND_HEIGHT, BACKGROUND_MOON_EXTRA_BITMAP, BACKGROUND_WIDTH, BACKGROUND_EXTRA_HEIGHT, isBackgroundMirror);
  }
}


static void drawBalls() {

  int i, j;
  //3 passes, paint big balls first
  for (j = BALL_BIG; j > 0; j--) {
    for (i = 0; i < MAX_BALLS; i++) {

      if (balls[i] != j) continue;

      helper.drawMaskedBitmap(ballsPosX[i] / SCREEN_MAGNIFICATION, ballsPosY[i] / SCREEN_MAGNIFICATION,
                              (ballFrame == 0 || ballFrame == 1) ? BALL_BITMAPS[balls[i]] : BALL_ROT_BITMAPS[balls[i]],
                              BALL_MASK_BITMAPS[balls[i]], BALL_SIZES[balls[i]], BALL_SIZES[balls[i]], (ballFrame == 0 || ballFrame == 2) ? 0 : MIRROR);
    }
  }

  //Rotate frames
  if (areBallsRotating && gameState == STATE_PLAYING && (arduboy.everyXFrames(6 / frameDrop))) ballFrame = (ballFrame + NUMBER_OF_BALL_FRAMES - 1) % NUMBER_OF_BALL_FRAMES;
}


static void drawRays() {

  int i;
  for (i = 0; i < MAX_RAYS; i++) {
    if (isRayVisible[i]) {
      helper.drawBitmap(rayPosX[i] / SCREEN_MAGNIFICATION, rayPosY[i] / SCREEN_MAGNIFICATION, RAY_BITMAP, RAY_WIDTH, RAY_HEIGHT, 0);
    }
  }
}


static void drawGift() {

  if (isGiftVisible && (giftTime < GIFT_STARTS_BLINKING_TIME || (giftTime / GIFT_BLINKING_TIME) % 2 == 0 )) {

    helper.drawMaskedBitmap(giftPosX / SCREEN_MAGNIFICATION, giftPosY / SCREEN_MAGNIFICATION,
                            (giftType == GIFT_LIFE) ? GIFT_LIFE_BITMAP :
                            (giftType == GIFT_DOUBLE_RAY) ? GIFT_DOUBLE_RAY_BITMAP :
                            (giftType == GIFT_NEGATIVE) ? GIFT_NEGATIVE_BITMAP :
                            (giftType == GIFT_MIRROR) ? GIFT_MIRROR_BITMAP :
                            (giftType == GIFT_DEATH) ? GIFT_DEATH_BITMAP :
                            (giftType == GIFT_SHIELD) ? GIFT_SHIELD_BITMAP :
                            GIFT_SLOW_BITMAP , GIFT_MASK_BITMAP, GIFT_SIZE, GIFT_SIZE, (giftType == GIFT_FAST) ? MIRROR^isMirrorEnabled : isMirrorEnabled);
  }

}


static void drawPlayer() {

  if (checkLevelComplete()) {
    helper.drawMaskedBitmap(playerPosX / SCREEN_MAGNIFICATION, playerPosY / SCREEN_MAGNIFICATION, PLAYER_HAPPY_BITMAP, PLAYER_HAPPY_MASK_BITMAP, PLAYER_WIDTH, PLAYER_HEIGHT, 0);
    return;
  }

  if (shieldTime > 0  && (shieldTime / GIFT_BLINKING_TIME) % 2 == 0 ) return;
  
  if (gameState == STATE_PLAYING && arduboy.pressed(RIGHT_BUTTON)) {
    helper.drawMaskedBitmap(playerPosX / SCREEN_MAGNIFICATION, playerPosY / SCREEN_MAGNIFICATION, PLAYER_RIGHT_BITMAP[(playerPosX / (FRAME_CHANGE * SCREEN_MAGNIFICATION)) % NUMBER_FRAMES], PLAYER_RIGHT_MASK_BITMAP[(playerPosX / (FRAME_CHANGE * SCREEN_MAGNIFICATION)) % NUMBER_FRAMES], PLAYER_WIDTH, PLAYER_HEIGHT, 0);
  } else if (gameState == STATE_PLAYING && arduboy.pressed(LEFT_BUTTON)) {
    helper.drawMaskedBitmap(playerPosX / SCREEN_MAGNIFICATION, playerPosY / SCREEN_MAGNIFICATION, PLAYER_RIGHT_BITMAP[(playerPosX / (FRAME_CHANGE * SCREEN_MAGNIFICATION)) % 2], PLAYER_RIGHT_MASK_BITMAP[(playerPosX / (FRAME_CHANGE * SCREEN_MAGNIFICATION)) % NUMBER_FRAMES], PLAYER_WIDTH, PLAYER_HEIGHT, MIRROR);
  } else {
    boolean showPlayerShoting = (millis() - rayLaunchedTime) < PLAYER_RAY_LAUNCH_FRAME_TIME;
    helper.drawMaskedBitmap(playerPosX / SCREEN_MAGNIFICATION, playerPosY / SCREEN_MAGNIFICATION, showPlayerShoting ? PLAYER_SHOT_BITMAP : PLAYER_BITMAP, showPlayerShoting ? PLAYER_SHOT_MASK_BITMAP : PLAYER_MASK_BITMAP , PLAYER_WIDTH, PLAYER_HEIGHT, 0);
  }

  if (isShieldEnabled){
   // helper.drawCircle(playerPosX / SCREEN_MAGNIFICATION + PLAYER_WIDTH / 2 , HEIGHT - PLAYER_HEIGHT / 4, 20 - PLAYER_HEIGHT / 4);
   //int offset = -1 + (millis() / 150 )%3;
   //if (gameState != STATE_PLAYING) offset = 0;
   //arduboy.drawCircle(playerPosX / SCREEN_MAGNIFICATION + PLAYER_WIDTH / 2 , HEIGHT - 1 - PLAYER_HEIGHT / 4, 20 - PLAYER_HEIGHT / 4 + offset, WHITE);
  // arduboy.drawCircle(playerPosX / SCREEN_MAGNIFICATION + PLAYER_WIDTH / 2 , HEIGHT - 1 - PLAYER_HEIGHT / 4, 21 - PLAYER_HEIGHT / 4, WHITE);
  if (gameState == STATE_PLAYING){
    helper.drawNoiseCircle(playerPosX / SCREEN_MAGNIFICATION + PLAYER_WIDTH / 2 , HEIGHT - PLAYER_HEIGHT / 2, 22 - PLAYER_HEIGHT / 2);
   // helper.drawNoiseCircle(playerPosX / SCREEN_MAGNIFICATION + PLAYER_WIDTH / 2 , HEIGHT - 1 - PLAYER_HEIGHT / 2, 23 - PLAYER_HEIGHT / 2);
  } else arduboy.drawCircle(playerPosX / SCREEN_MAGNIFICATION + PLAYER_WIDTH / 2 , HEIGHT - PLAYER_HEIGHT / 2, 22 - PLAYER_HEIGHT / 2, WHITE);
   //arduboy.drawCircle(playerPosX / SCREEN_MAGNIFICATION + PLAYER_WIDTH / 2 , HEIGHT - 1 - PLAYER_HEIGHT / 2, 22 - PLAYER_HEIGHT / 2, WHITE);
  }

}

static void drawItems() {

  numItems = 0;
  const unsigned int INIT_POS = 3;

  if (isNoGravityEnabled) {
    helper.drawMaskedBitmap(INIT_POS, INIT_POS,  GIFT_NO_GRAVITY_BITMAP, GIFT_MASK_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    numItems++;
  }

  if (isMirrorEnabled) {
    helper.drawMaskedBitmap(INIT_POS + numItems * (GIFT_SIZE + 2) , INIT_POS, GIFT_MIRROR_BITMAP, GIFT_MASK_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    numItems++;
  }

  if (isNegativeEnabled) {
    helper.drawMaskedBitmap(INIT_POS + numItems * (GIFT_SIZE + 2) , INIT_POS, GIFT_NEGATIVE_BITMAP, GIFT_MASK_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    numItems++;
  }

  if (modifiedSpeed != MODIFIED_SPEED_NONE) {
    helper.drawMaskedBitmap(INIT_POS + numItems * (GIFT_SIZE + 2) , INIT_POS,  GIFT_SLOW_BITMAP, GIFT_MASK_BITMAP, GIFT_SIZE, GIFT_SIZE, (modifiedSpeed == MODIFIED_SPEED_SLOW) ? 0 : MIRROR);
    numItems++;
  }

  if (isShieldEnabled) {
    helper.drawMaskedBitmap(INIT_POS + numItems * (GIFT_SIZE + 2) , INIT_POS,  GIFT_SHIELD_BITMAP, GIFT_MASK_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    numItems++;
  }
  
  if (isDoubleRayEnabled) {
    helper.drawMaskedBitmap(INIT_POS + numItems * (GIFT_SIZE + 2) , INIT_POS,  GIFT_DOUBLE_RAY_BITMAP, GIFT_MASK_BITMAP, GIFT_SIZE, GIFT_SIZE, 0);
    numItems++;
  }

}


static void drawLives() {

  int i;
  for (i = 0; i < lives; i++) {

    helper.drawBitmap(WIDTH - (HEART_SIZE + 4) * (i + 1) , 3, HEART_BITMAP, HEART_SIZE, HEART_SIZE, 0);

    // If more than 5 lives only draw one and a number with the total
    if (lives > 5 || numItems > 4) {
      const unsigned char OFFSET = 110;

      //arduboy.setCursor(OFFSET, 3);
      //arduboy.print(F("x"));
      helper.print(STRING_X, OFFSET, 3);
      unsigned char position = OFFSET - 7 - 6 * (getNumberOfChars(lives) - 1);

      arduboy.setCursor(position, 3);
      arduboy.print(lives);

      break;
    }
  }
}

static void drawBanner (const char* text) {

  int textLength = strlen_P(text);

  boolean bigBanner = textLength > 10;
  uint8_t bannerWidth = bigBanner ? BIG_BANNER_WIDTH : SMALL_BANNER_WIDTH;
  uint8_t bannerHeight = bigBanner ? BIG_BANNER_HEIGHT : SMALL_BANNER_HEIGHT;
/*
  helper.drawMaskedBitmap((WIDTH - bannerWidth) / 2, (HEIGHT - bannerHeight) / 2,
                          bigBanner ? BIG_BANNER_BITMAP : SMALL_BANNER_BITMAP,
                          bigBanner ? BIG_BANNER_MASK_BITMAP : SMALL_BANNER_MASK_BITMAP,
                          bannerWidth, bannerHeight, 0);*/
  drawCenteredBanner(bannerWidth);                       
  //arduboy.setCursor((WIDTH - textLength * 6 - 1) / 2 + 1, (HEIGHT - 7) / 2);
/*
  int i;
  char myChar;
  for (i = 0; i < textLength; i++) {
    myChar =  pgm_read_byte(text + i);
    arduboy.print(myChar);
  }
*/
  helper.print(text, (WIDTH - textLength * 6 - 1) / 2 + 1, (HEIGHT - 7) / 2);
}

static void drawLevelBanner (unsigned char level) {

  int textLength = 6 + getNumberOfChars(level);
  //helper.drawMaskedBitmap((WIDTH - SMALL_BANNER_WIDTH) / 2, (HEIGHT - SMALL_BANNER_HEIGHT) / 2, SMALL_BANNER_BITMAP, SMALL_BANNER_MASK_BITMAP, SMALL_BANNER_WIDTH, SMALL_BANNER_HEIGHT, 0);
  drawCenteredBanner(SMALL_BANNER_WIDTH);
  //arduboy.setCursor((WIDTH - textLength * 6 - 1) / 2 + 1, (HEIGHT - 7) / 2);
  //arduboy.print(F("LEVEL "));
  helper.print(STRING_LEVEL, (WIDTH - textLength * 6 - 1) / 2 + 1, (HEIGHT - 7) / 2);
  arduboy.print(level);
}

static void drawCenteredBanner (uint8_t lenght){

  uint8_t initX = (WIDTH - lenght) / 2;
  const uint8_t Y = (HEIGHT - BANNER_HEAD_HEIGHT) / 2;

  helper.drawMaskedBitmap(initX, Y, BANNER_HEAD_BITMAP, BANNER_HEAD_MASK_BITMAP, BANNER_HEAD_WIDTH, BANNER_HEAD_HEIGHT, 0);
  initX += BANNER_HEAD_WIDTH;

  uint8_t bodySize = lenght - 2 * BANNER_HEAD_WIDTH;

  for (int i = 0; i < bodySize; i++){
    helper.drawMaskedBitmap(initX, Y, BANNER_BODY_BITMAP, BANNER_BODY_MASK_BITMAP, BANNER_BODY_WIDTH, BANNER_BODY_HEIGHT, 0);    
    initX++;
  }

  helper.drawMaskedBitmap(initX, Y, BANNER_HEAD_BITMAP, BANNER_HEAD_MASK_BITMAP, BANNER_HEAD_WIDTH, BANNER_HEAD_HEIGHT, MIRROR);

  
}

void loseShield(){
        isShieldEnabled = false;
        shieldTime = SHIELD_DISAPPEARS_TIME;
  
}

