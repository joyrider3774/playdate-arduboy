/*
  EEPROM.h - EEPROM library
  Original Copyright (c) 2006 David A. Mellis.  All right reserved.
  New version by Christopher Andrews 2015.
  AVR eeprom.h compatibility layer added for Arduboy/AVR port compatibility.
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
#include <string.h>

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
    else
    {
        //enable audio by default on new eeproms
        _eeprom[2] = 1;
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


/***
    AVR <avr/eeprom.h> compatibility layer.

    On AVR, addresses are passed as uint8_t* (EEMEM pointers). Here we treat
    those pointers as byte offsets cast to uintptr_t, matching the typical
    Arduboy pattern of:
        uint8_t myVar EEMEM;
        eeprom_read_byte(&myVar);   // &myVar == offset into EEPROM space
***/

// EEMEM attribute — on AVR this places variables in the .eeprom section.
// Here it is a no-op; variables live in normal RAM and their address is
// used as the EEPROM offset (cast to uintptr_t).
#ifndef EEMEM
#define EEMEM
#endif

// Helper: convert an AVR-style EEMEM pointer to a flat integer offset.
// Callers typically pass &some_eemem_var whose address is already the
// intended logical EEPROM offset (as done in most Arduboy sketches).
#define _EE_ADDR(p) ((int)(uintptr_t)(p))

// --------------------------------------------------------------------------
// Read functions
// --------------------------------------------------------------------------

static __attribute__((unused)) inline uint8_t eeprom_read_byte(const uint8_t *addr)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx >= EEPROM_SIZE) return 0xFF;
    return _eeprom[idx];
}

static __attribute__((unused)) inline uint16_t eeprom_read_word(const uint16_t *addr)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + 1 >= EEPROM_SIZE) return 0xFFFF;
    uint16_t val;
    memcpy(&val, &_eeprom[idx], sizeof(val));
    return val;
}

static __attribute__((unused)) inline uint32_t eeprom_read_dword(const uint32_t *addr)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + 3 >= EEPROM_SIZE) return 0xFFFFFFFFUL;
    uint32_t val;
    memcpy(&val, &_eeprom[idx], sizeof(val));
    return val;
}

static __attribute__((unused)) inline float eeprom_read_float(const float *addr)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + (int)sizeof(float) - 1 >= EEPROM_SIZE) return 0.0f;
    float val;
    memcpy(&val, &_eeprom[idx], sizeof(val));
    return val;
}

static __attribute__((unused)) inline void eeprom_read_block(void *dst, const void *src, size_t n)
{
    _eeprom_load();
    int idx = _EE_ADDR(src);
    if (idx < 0 || idx + (int)n > EEPROM_SIZE) return;
    memcpy(dst, &_eeprom[idx], n);
}

// --------------------------------------------------------------------------
// Write functions (always write, no update check)
// --------------------------------------------------------------------------

static __attribute__((unused)) inline void eeprom_write_byte(uint8_t *addr, uint8_t val)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx >= EEPROM_SIZE) return;
    _eeprom[idx] = val;
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_write_word(uint16_t *addr, uint16_t val)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + 1 >= EEPROM_SIZE) return;
    memcpy(&_eeprom[idx], &val, sizeof(val));
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_write_dword(uint32_t *addr, uint32_t val)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + 3 >= EEPROM_SIZE) return;
    memcpy(&_eeprom[idx], &val, sizeof(val));
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_write_float(float *addr, float val)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + (int)sizeof(float) - 1 >= EEPROM_SIZE) return;
    memcpy(&_eeprom[idx], &val, sizeof(val));
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_write_block(const void *src, void *dst, size_t n)
{
    _eeprom_load();
    int idx = _EE_ADDR(dst);
    if (idx < 0 || idx + (int)n > EEPROM_SIZE) return;
    memcpy(&_eeprom[idx], src, n);
    _eeprom_save();
}

// --------------------------------------------------------------------------
// Update functions (write only if value differs — saves unnecessary I/O)
// --------------------------------------------------------------------------

static __attribute__((unused)) inline void eeprom_update_byte(uint8_t *addr, uint8_t val)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx >= EEPROM_SIZE) return;
    if (_eeprom[idx] == val) return;
    _eeprom[idx] = val;
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_update_word(uint16_t *addr, uint16_t val)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + 1 >= EEPROM_SIZE) return;
    if (memcmp(&_eeprom[idx], &val, sizeof(val)) == 0) return;
    memcpy(&_eeprom[idx], &val, sizeof(val));
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_update_dword(uint32_t *addr, uint32_t val)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + 3 >= EEPROM_SIZE) return;
    if (memcmp(&_eeprom[idx], &val, sizeof(val)) == 0) return;
    memcpy(&_eeprom[idx], &val, sizeof(val));
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_update_float(float *addr, float val)
{
    _eeprom_load();
    int idx = _EE_ADDR(addr);
    if (idx < 0 || idx + (int)sizeof(float) - 1 >= EEPROM_SIZE) return;
    if (memcmp(&_eeprom[idx], &val, sizeof(val)) == 0) return;
    memcpy(&_eeprom[idx], &val, sizeof(val));
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_update_block(const void *src, void *dst, size_t n)
{
    _eeprom_load();
    int idx = _EE_ADDR(dst);
    if (idx < 0 || idx + (int)n > EEPROM_SIZE) return;
    if (memcmp(&_eeprom[idx], src, n) == 0) return;
    memcpy(&_eeprom[idx], src, n);
    _eeprom_save();
}

static __attribute__((unused)) inline void eeprom_busy_wait()
{

}

#endif