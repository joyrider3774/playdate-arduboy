#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"

class Enemy; // forward declaration for pointer use in declarations below

/* ----------------------------------------------------------------------------
 *  Return the upper 4 bits of a byte.
 */
inline uint8_t leftValue(uint8_t val) {

  return val >> 4; 
      
}


/* ----------------------------------------------------------------------------
 *  Return the lower 4 bits of a byte.
 */
inline uint8_t rightValue(uint8_t val) {

  return val & 0x0F; 
      
}

// ---------------------------------------------------------------------------------
// Extract individual digits of a uint8_t
//
template< size_t size >
void extractDigits(uint8_t (&buffer)[size], uint8_t value) {

  for (uint8_t i = 0; i < size; ++i) {
    buffer[i] = value % 10;
    value /= 10;
  }

}

// ---------------------------------------------------------------------------------
// Extract individual digits of a uint16_t
//
template< size_t size >
void extractDigits(uint8_t (&buffer)[size], uint16_t value) {

  for(uint8_t i = 0; i < size; ++i) {
    buffer[i] = value % 10;
    value /= 10;
  }

}

// ---------------------------------------------------------------------------------
// Can be stood on by the player ?
//
// The player cannot stand on brick that has been dug out or is repairing itself.
//
boolean canBeStoodOn(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY);

// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
boolean isSolid(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can the player fall onto these elements ?
//
// Elements should be the same as the 'canBeStoodOn' with the addition of the rail.
//
boolean canBeFallenOn(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can the space be climbed on by the player ?
//
boolean canBeClimbedOn(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can the space be occupied by the player ?
//
// Can the player enter the square - ie. is it empty or something that can be climbed on?
//
boolean canBeOccupied(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can the space be fallen into by the player ?
//
boolean canBeFallenInto(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY);

// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
boolean canBeStoodOnBasic_Enemy(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
// The enemy can stand on bricks that have been dug out or are reforming.  Of 
// course they will fall into these if they do step on them.
//
boolean canBeStoodOn_Enemy(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can the space be occupied by the enemy ?
//
// Can the player enter the square - ie. is it empty or something that can be climbed on?
//
boolean canBeOccupiedBasic_Enemy(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can the space be occupied by the enemy ?
//
// Can the player enter the square - ie. is it empty or something that can be climbed on?
//
boolean canBeOccupied_Enemy(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can a falling enemy continue to fall ?  Should stop if they have fallen into a dug hole.
//
boolean canContinueToFall_Enemy(LevelElement levelElement);

// ---------------------------------------------------------------------------------
// Can the space be fallen into by the enemy ?
//
boolean canBeFallenInto_Enemy(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY);
