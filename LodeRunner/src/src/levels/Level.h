#pragma once

#include "../utils/Arduboy2Ext.h"

#include "../utils/Enums.h"
#include "../characters/Player.h"
#include "../characters/Enemy.h"

#define ENCRYPTION_TYPE_RLE_ROW 0
#define ENCRYPTION_TYPE_RLE_COL 1
#define ENCRYPTION_TYPE_GRID 2

class Level {

  public: 

    Level() {};
        
    LevelElement getLevelData(const uint8_t x, const uint8_t y);
    void loadLevel(Player *player, Enemy enemies[]);
    void pickupGold();

    uint8_t getHeight();
    uint8_t getWidth();
    int16_t getXOffset();
    int16_t getYOffset();
    int8_t getXOffsetDelta();
    int8_t getYOffsetDelta();
    uint8_t getLevelNumber();
    uint8_t getGoldLeft();
    uint8_t getLevelLadderElementCount();
    LevelPoint getLevelLadderElement(const uint8_t index);
    LevelPoint getNextReentryPoint();
    
    void setLevelData(const uint8_t x, const uint8_t y, const LevelElement levelElement);
    void setXOffset(int16_t val);
    void setYOffset(int16_t val);
    void setXOffsetDelta(int8_t val);
    void setYOffsetDelta(int8_t val);
    void setLevelNumber(uint8_t val);
    void setGoldLeft(uint8_t val);

  private:

    static const uint8_t _width = 14;
    static const uint8_t _height = 16;

    int16_t _xOffset;
    int16_t _yOffset;
    int8_t _xOffsetDelta;
    int8_t _yOffsetDelta;
    uint8_t _levelData[_width][_height];
    uint8_t _levelNumber;
    uint8_t _goldLeft;

    uint8_t _levelLadderElementCount;
    uint8_t _reentryPointIndex;

    LevelPoint _levelLadder[18];
    LevelPoint _reentryPoint[4];

};

//--------------------------------------------------------------------------------------------------------------------------

inline uint8_t Level::getWidth() {
    return _width;
}

inline uint8_t Level::getHeight() {
    return _height;
}

inline int16_t Level::getXOffset() {
    return _xOffset;
}

inline int16_t Level::getYOffset() {
    return _yOffset;
}

inline int8_t Level::getXOffsetDelta() {
    return _xOffsetDelta;
}

inline int8_t Level::getYOffsetDelta() {
    return _yOffsetDelta;
}

inline uint8_t Level::getLevelNumber() {
    return _levelNumber;
}

inline uint8_t Level::getGoldLeft() {
    return _goldLeft;
}

inline uint8_t Level::getLevelLadderElementCount() {
    return _levelLadderElementCount;
}

inline void Level::setXOffset(int16_t val) {
    _xOffset = val;
}

inline void Level::setYOffset(int16_t val) {
    _yOffset = val;
}

inline void Level::setXOffsetDelta(int8_t val) {
    _xOffsetDelta = val;
}

inline void Level::setYOffsetDelta(int8_t val) {
    _yOffsetDelta = val;
}

inline void Level::setLevelNumber(uint8_t val) {
    _levelNumber = val;
}

inline void Level::setGoldLeft(uint8_t val) {
    _goldLeft = val;
}

inline LevelPoint Level::getLevelLadderElement(const uint8_t index) {
  return _levelLadder[index];
}

inline LevelPoint Level::getNextReentryPoint() {
  return _reentryPoint[_reentryPointIndex];
  _reentryPointIndex = (_reentryPointIndex == 3 ? 0 : _reentryPointIndex + 1);
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
//  Get level element at position x and y ..
//
inline LevelElement Level::getLevelData(const uint8_t x, const uint8_t y) {

  if ((x / 2) >= _width) return LevelElement::Brick;
  if (y == 255) return LevelElement::Blank;
  if (y >= _height) return LevelElement::Solid;

  if (x % 2 == 0) {

    return static_cast<LevelElement>(_levelData[x / 2][y] >> 4); 

  }
  else {

    return static_cast<LevelElement>(_levelData[x / 2][y] & 0x0F); 

  }

  return LevelElement::Brick;
      
}


// -----------------------------------------------------------------------------------------------
//  Set level element at position x and y ..
//
inline void Level::setLevelData(const uint8_t x, const uint8_t y, const LevelElement levelElement) {

  if (x % 2 == 0) {

    _levelData[x / 2][y] = (_levelData[x / 2][y] & 0x0f) | (static_cast<uint8_t>(levelElement) << 4); 

  }
  else {

    _levelData[x / 2][y] = (_levelData[x / 2][y] & 0xf0) | static_cast<uint8_t>(levelElement); 
    
  }
      
}


// -----------------------------------------------------------------------------------------------
//  Update the level when the last gold is collected ..
//
inline void Level::pickupGold() {

  if (_goldLeft > 0) _goldLeft--;

  if (_goldLeft == 0) {

    // Update map with level ladder ..

    for (uint8_t x = 0; x < _levelLadderElementCount; x++) {

      LevelPoint lp = _levelLadder[x];

      Level::setLevelData(lp.x, lp.y, LevelElement::Ladder);

    }

  }

}