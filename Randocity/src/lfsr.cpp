#include "lfsr.h"

void LFSR::forward() {
  bool lsb = (lfsr & lsbBit) != 0;
  lfsr = ((lfsr>>1) ^ ((-lsb) & forwardPoly));   
}

void LFSR::backward() {
  bool msb = (lfsr & msbBit) != 0;
  lfsr = mask & ((lfsr<<1) ^ ((-msb) & backwardPoly));   
}

void LFSR::forward(uint16_t n) {
  for(uint16_t i=0; i<n; i++) forward();
}

void LFSR::backward(uint16_t n) {
  for(uint16_t i=0; i<n; i++) backward();
}

void LFSR::forward1024() {
  uint32_t prev = lfsr;
  lfsr=0;
  if (prev & 0x00001) lfsr ^= 0x94cf1;
  if (prev & 0x00002) lfsr ^= 0x299eb;
  if (prev & 0x00004) lfsr ^= 0x533d6;
  if (prev & 0x00008) lfsr ^= 0xa67ac;
  if (prev & 0x00010) lfsr ^= 0x4cf51;
  if (prev & 0x00020) lfsr ^= 0x99ea2;
  if (prev & 0x00040) lfsr ^= 0x33d4d;
  if (prev & 0x00080) lfsr ^= 0x67a9a;
  if (prev & 0x00100) lfsr ^= 0xcf534;
  if (prev & 0x00200) lfsr ^= 0x9ea61;
  if (prev & 0x00400) lfsr ^= 0x3d4cb;
  if (prev & 0x00800) lfsr ^= 0x7a996;
  if (prev & 0x01000) lfsr ^= 0xf532c;
  if (prev & 0x02000) lfsr ^= 0xea651;
  if (prev & 0x04000) lfsr ^= 0xd4cab;
  if (prev & 0x08000) lfsr ^= 0xa995f;
  if (prev & 0x10000) lfsr ^= 0x532b7;
  if (prev & 0x20000) lfsr ^= 0xa656e;
  if (prev & 0x40000) lfsr ^= 0x4cad5;
  if (prev & 0x80000) lfsr ^= 0x995aa;
}

// forward 0x100000-1-1024 = backward 1024
void LFSR::backward1024() {
  uint32_t prev = lfsr;
  lfsr=0;
  if (prev & 0x00001) lfsr ^= 0x46103;
  if (prev & 0x00002) lfsr ^= 0x8c206;
  if (prev & 0x00004) lfsr ^= 0x18405;
  if (prev & 0x00008) lfsr ^= 0x3080a;
  if (prev & 0x00010) lfsr ^= 0x61014;
  if (prev & 0x00020) lfsr ^= 0xc2028;
  if (prev & 0x00040) lfsr ^= 0x84059;
  if (prev & 0x00080) lfsr ^= 0x080bb;
  if (prev & 0x00100) lfsr ^= 0x10176;
  if (prev & 0x00200) lfsr ^= 0x202ec;
  if (prev & 0x00400) lfsr ^= 0x405d8;
  if (prev & 0x00800) lfsr ^= 0x80bb0;
  if (prev & 0x01000) lfsr ^= 0x01769;
  if (prev & 0x02000) lfsr ^= 0x02ed2;
  if (prev & 0x04000) lfsr ^= 0x05da4;
  if (prev & 0x08000) lfsr ^= 0x0bb48;
  if (prev & 0x10000) lfsr ^= 0x17690;
  if (prev & 0x20000) lfsr ^= 0x2ed20;
  if (prev & 0x40000) lfsr ^= 0x5da40;
  if (prev & 0x80000) lfsr ^= 0xbb480;
}
