#pragma once

#include "../../Enums.h"

extern const char * EnemyNames[];

inline const char * getEnemyName(const EnemyType & type) {

	return EnemyNames[static_cast<uint8_t>(type)];
	
}
