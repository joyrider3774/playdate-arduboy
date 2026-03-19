#pragma once
#include "Arduboy2Common.h"
#include <stdio.h>

// Minimal Arduino Print class stub for Playdate
class Print {
public:
    virtual size_t write(uint8_t) = 0;

    size_t write(const char *str) {
        size_t n = 0;
        while (*str) n += write((uint8_t)*str++);
        return n;
    }

    size_t print(const char *str)     { return write(str); }
    size_t print(char c)              { return write((uint8_t)c); }
    size_t print(int n)               { char buf[16]; sprintf(buf, "%d", n); return write(buf); }
    size_t print(unsigned int n)      { char buf[16]; sprintf(buf, "%u", n); return write(buf); }
    size_t print(long n)              { char buf[16]; sprintf(buf, "%ld", n); return write(buf); }
    size_t print(unsigned long n)     { char buf[16]; sprintf(buf, "%lu", n); return write(buf); }
    size_t print(uint8_t n)           { return print((unsigned int)n); }
    size_t print(float n, int dec=2)  { char buf[24]; char fmt[8]; sprintf(fmt, "%%.%df", dec); sprintf(buf, fmt, (double)n); return write(buf); }
    size_t print(double n, int dec=2) { return print((float)n, dec); }

    size_t println(const char *str) { size_t n = print(str); write('\n'); return n + 1; }
    size_t println()                { return write('\n'); }
};
