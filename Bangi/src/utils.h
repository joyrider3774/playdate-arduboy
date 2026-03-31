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
int getNumberOfChars (int number) {
  return ((number / 100 > 0) ? 3 :
          (number / 10 > 0) ? 2 :
          1);
}

//Fast pseudoo-random number generator, it's ok for noise
#define STATE_BYTES 7
#define MULT 0x13B /* for STATE_BYTES==6 only */
#define MULT_LO (MULT & 255)
#define MULT_HI (MULT & 256)

uint8_t rand8() {
  static uint8_t state[STATE_BYTES] = { 0x87, 0xdd, 0xdc, 0x10, 0x35, 0xbc, 0x5c };
  static uint16_t c = 0x42;
  static int i = 0;
  uint16_t t;
  uint8_t x;

  x = state[i];
  t = (uint16_t)x * MULT_LO + c;
  c = t >> 8;

#if MULT_HI
  c += x;
#endif

  x = t & 255;
  state[i] = x;
  if (++i >= sizeof(state))
    i = 0;
  return x;
}

