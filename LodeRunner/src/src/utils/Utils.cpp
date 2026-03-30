#include "../../globals.h"
#include "Utils.h"

// ---------------------------------------------------------------------------------
// Can be stood on by the player ?
//
boolean canBeStoodOn(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
    case LevelElement::Ladder:
      return true;

    default:

	  // Is the player standing on top of an enemy?

      for (uint8_t x = 0; x < NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getEnabled()) {

          if (enemy->getX() == positionX * GRID_SIZE && enemy->getY() == positionY * GRID_SIZE) {

            return true; 

          }

        }

      }

	    return false;

  }

}


// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
boolean isSolid(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
      return true;

    default:
      return false;

  }

}

// ---------------------------------------------------------------------------------
// Can the player fall onto these elements ?
//
boolean canBeFallenOn(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
    case LevelElement::Rail:
    case LevelElement::Ladder:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be climbed on by the player ?
//
boolean canBeClimbedOn(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Ladder:
    case LevelElement::Rail:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be occupied by the player ?
//
boolean canBeOccupied(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::Ladder:
    case LevelElement::Rail:
    case LevelElement::Gold:
    case LevelElement::FallThrough:
    case LevelElement::Brick_1 ... LevelElement::Brick_Close_4:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be fallen into by the player ?
//
boolean canBeFallenInto(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::FallThrough:
    case LevelElement::Rail:
    case LevelElement::Gold:
      return true;

    case LevelElement::Brick_1:
    case LevelElement::Brick_2:
    case LevelElement::Brick_3:
    case LevelElement::Brick_4:
    case LevelElement::Brick_Transition:
    case LevelElement::Brick_Close_1:
    case LevelElement::Brick_Close_2:
    case LevelElement::Brick_Close_3:
    case LevelElement::Brick_Close_4:

      for (uint8_t x = 0; x < NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getEnabled()) {

          if (enemy->getStance() == PlayerStance::Falling && enemy->getX() == positionX * GRID_SIZE && enemy->getY() == positionY * GRID_SIZE) {

            return false; 

          }

        }

      }

      return true;

    default:
      return false;

  }

}

// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
boolean canBeStoodOnBasic_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
    case LevelElement::Ladder:
      return true;

    default:
      return false;

  }

}

// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
boolean canBeStoodOn_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
    case LevelElement::Ladder:
    case LevelElement::Brick_1:
    case LevelElement::Brick_2:
    case LevelElement::Brick_3:
    case LevelElement::Brick_4:
    case LevelElement::Brick_Transition:
    case LevelElement::Brick_Close_1:
    case LevelElement::Brick_Close_2:
    case LevelElement::Brick_Close_3:
    case LevelElement::Brick_Close_4:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be occupied by the enemy ?
//
boolean canBeOccupiedBasic_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::Ladder:
    case LevelElement::Rail:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be occupied by the enemy ?
//
boolean canBeOccupied_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::Ladder:
    case LevelElement::Rail:
    case LevelElement::FallThrough:
    case LevelElement::Gold:
    case LevelElement::Brick_1:
    case LevelElement::Brick_2:
    case LevelElement::Brick_3:
    case LevelElement::Brick_4:
    case LevelElement::Brick_Transition:
    case LevelElement::Brick_Close_1:
    case LevelElement::Brick_Close_2:
    case LevelElement::Brick_Close_3:
    case LevelElement::Brick_Close_4:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can a falling enemy continue to fall ?
//
boolean canContinueToFall_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::FallThrough:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be fallen into by the enemy ?
//
boolean canBeFallenInto_Enemy(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::FallThrough:
      return true;

    case LevelElement::Brick_1:
    case LevelElement::Brick_2:
    case LevelElement::Brick_3:
    case LevelElement::Brick_4:
    case LevelElement::Brick_Transition:
    case LevelElement::Brick_Close_1:
    case LevelElement::Brick_Close_2:
    case LevelElement::Brick_Close_3:
//  case LevelElement::Brick_Close_4:

      // Is another enemy in the position already ?

      for (uint8_t x = 0; x < NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getEnabled()) {

          if (enemy->getX() == positionX * GRID_SIZE && enemy->getY() == positionY * GRID_SIZE) {

            return false; 

          }

        }

      }

      return true;

    default:
      return false;

  }

}
