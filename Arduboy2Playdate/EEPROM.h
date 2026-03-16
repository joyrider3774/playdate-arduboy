/*
  EEPROM.h - EEPROM library
  Original Copyright (c) 2006 David A. Mellis.  All right reserved.
  New version by Christopher Andrews 2015.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROM_h
#define EEPROM_h

extern "C" {
#include <stdint.h>
#include "pd_api.h"
extern PlaydateAPI* pd;
}

/***
    EEPROMClass class.
    This object represents the entire EEPROM space.
    It wraps the functionality of EEPtr and EERef into a basic interface.
    This class is also 100% backwards compatible with earlier Arduino core releases.
***/

#include <string>

extern PlaydateAPI* pd;

#define EEPROM_SIZE 1024
#define EEPROM_FILE "eeprom.bin"

inline uint8_t _eeprom[EEPROM_SIZE];
inline bool _eeprom_loaded = false;

static inline void _eeprom_load() {
    if (_eeprom_loaded) return;
    _eeprom_loaded = true;

    SDFile* f = pd->file->open(EEPROM_FILE, kFileReadData);
    if (f) {
        pd->file->read(f, _eeprom, EEPROM_SIZE);
        pd->file->close(f);
    }
}

static inline void _eeprom_save() {
    SDFile* f = pd->file->open(EEPROM_FILE, kFileWrite);
    if (f) {
        pd->file->write(f, _eeprom, EEPROM_SIZE);
        pd->file->close(f);
    }
}

struct EEPROMClass {

    //Basic user access methods.

    uint8_t read(int idx)
    {
        _eeprom_load();
        if ((idx >= EEPROM_SIZE) || (idx < 0))
            return 0;
        return _eeprom[idx];
    }

    void write(int idx, uint8_t val)
    {
        _eeprom_load();
        if ((idx >= EEPROM_SIZE) || (idx < 0))
            return;
        _eeprom[idx] = val;
        _eeprom_save();
    }

    void update(int idx, uint8_t val)
    {
        _eeprom_load();
        if ((idx >= EEPROM_SIZE) || (idx < 0))
            return;
        if (_eeprom[idx] != val) {
            _eeprom[idx] = val;
            _eeprom_save();
        }
    }

    uint16_t length()
    {
        return EEPROM_SIZE;
    }

    //Functionality to 'get' and 'put' objects to and from EEPROM.
    template< typename T > T &get(int idx, T &t)
    {
        _eeprom_load();
        if (idx < 0 || idx + (int)sizeof(T) > EEPROM_SIZE) return t;
        uint8_t* ptr = (uint8_t*)&t;
        for (int i = 0; i < (int)sizeof(T); i++)
            ptr[i] = _eeprom[idx + i];
        return t;
    }

    template< typename T > const T &put(int idx, const T &t)
    {
        _eeprom_load();
        if (idx < 0 || idx + (int)sizeof(T) > EEPROM_SIZE) return t;
        const uint8_t* ptr = (const uint8_t*)&t;
        for (int i = 0; i < (int)sizeof(T); i++)
            _eeprom[idx + i] = ptr[i];
        _eeprom_save();
        return t;
    }
};

static EEPROMClass __attribute__((unused)) EEPROM;
#endif