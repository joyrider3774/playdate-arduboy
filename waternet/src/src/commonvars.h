#ifndef commonvars_h
#define commonvars_h

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <stdint.h>

constexpr uint8_t maxBoardWidth = 10U;
constexpr uint8_t  maxBoardHeight = 8U;

constexpr uint8_t  maxBoardBgWidth = 10U;
constexpr uint8_t  maxBoardBgHeight = 8U;

constexpr uint8_t  maxBoardSize = maxBoardWidth * maxBoardHeight;

constexpr uint8_t  tileSize = 8;

constexpr uint8_t  gsGame = 0;
constexpr uint8_t  gsTitle = 1;
constexpr uint8_t  gsLevelSelect = 2;
constexpr uint8_t  gsLevelsCleared = 3;
constexpr uint8_t  gsHelpRotate = 4;
constexpr uint8_t  gsHelpRotate2 = 5;
constexpr uint8_t  gsHelpRotate3 = 6;
constexpr uint8_t  gsHelpRotateSlide = 7;
constexpr uint8_t  gsHelpRotateSlide2 = 8;
constexpr uint8_t  gsHelpRotateSlide3 = 9;
constexpr uint8_t  gsHelpRotateSlide4 = 10;
constexpr uint8_t  gsHelpSlide = 11;
constexpr uint8_t  gsHelpSlide2 = 12;
constexpr uint8_t  gsHelpSlide3 = 13;
constexpr uint8_t  gsIntro = 14;

constexpr uint8_t  gsInitDiff = 50;

constexpr uint8_t  gsInitGame = gsInitDiff + gsGame;
constexpr uint8_t  gsInitTitle = gsInitDiff + gsTitle;
constexpr uint8_t  gsInitLevelSelect = gsInitDiff + gsLevelSelect;
constexpr uint8_t  gsInitLevelsCleared = gsInitDiff + gsLevelsCleared;
constexpr uint8_t  gsInitHelpRotate = gsInitDiff + gsHelpRotate;
constexpr uint8_t  gsInitHelpRotate2 = gsInitDiff + gsHelpRotate2;
constexpr uint8_t  gsInitHelpRotate3 = gsInitDiff + gsHelpRotate3;
constexpr uint8_t  gsInitHelpRotateSlide = gsInitDiff + gsHelpRotateSlide;
constexpr uint8_t  gsInitHelpRotateSlide2 = gsInitDiff + gsHelpRotateSlide2;
constexpr uint8_t  gsInitHelpRotateSlide3 = gsInitDiff + gsHelpRotateSlide3;
constexpr uint8_t  gsInitHelpRotateSlide4 = gsInitDiff + gsHelpRotateSlide4;
constexpr uint8_t  gsInitHelpSlide = gsInitDiff + gsHelpSlide;
constexpr uint8_t  gsInitHelpSlide2 = gsInitDiff + gsHelpSlide2;
constexpr uint8_t  gsInitHelpSlide3 = gsInitDiff + gsHelpSlide3;
constexpr uint8_t  gsInitIntro = gsInitDiff + gsIntro;


constexpr uint8_t  diffVeryEasy = 0;
constexpr uint8_t  diffEasy = 1;
constexpr uint8_t  diffNormal = 2;
constexpr uint8_t  diffHard = 3;
constexpr uint8_t  diffVeryHard = 4;
constexpr uint8_t  diffRandom = 5;
constexpr uint8_t  diffCount = 6;

constexpr uint8_t  gmRotate = 0;
constexpr uint8_t  gmSlide = 1;
constexpr uint8_t  gmRotateSlide = 2;
constexpr uint8_t  gmCount = 3;

constexpr uint8_t  mmStartGame = 0;
constexpr uint8_t  mmHelp = 1;
constexpr uint8_t  mmOptions = 2;
constexpr uint8_t  mmCredits = 3;
constexpr uint8_t  mmCount = 4;

constexpr uint8_t  opMusic = 0;
constexpr uint8_t  opSound = 1;
constexpr uint8_t  opCount = 2;

constexpr uint8_t  tsMainMenu = 0;
constexpr uint8_t  tsGameMode = 1;
constexpr uint8_t  tsDifficulty = 2;
constexpr uint8_t  tsOptions = 3;
constexpr uint8_t  tsCredits = 4;

constexpr uint8_t  levelCount = 25;

constexpr uint8_t  arrowDown = 122;
constexpr uint8_t  arrowUp = 120;
constexpr uint8_t  arrowLeft = 123;
constexpr uint8_t  arrowRight = 121;
constexpr uint8_t  leftMenu = 118;

constexpr uint8_t  frameRate = 15;

extern uint8_t startPos, menuPos, 
        maxLevel, selectedLevel, boardX, boardY, difficulty, 
        gameState, boardWidth, boardHeight, boardSize,
        levelDone, titleStep, gameMode, posAdd, 
        debugMode, mainMenu, option, needRedraw;
        
extern int16_t selectionX, selectionY;
extern uint16_t moves;
extern unsigned long randomSeedGame;
extern unsigned char level[maxBoardSize];
extern Arduboy2Base arduboy;
extern ArduboyTones arduboySound;
#endif