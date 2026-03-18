#ifndef _ATMLIB_H_
#define _ATMLIB_H_
#include <stddef.h>
#include <inttypes.h>
#include <Arduboy2Common.h>

#define CH_ZERO             0
#define CH_ONE              1
#define CH_TWO              2
#define CH_THREE            3

extern uint8_t trackCount;
extern const uint16_t *trackList;
extern const uint8_t *trackBase;
extern uint8_t pcm;
extern bool half;

// cia = samples per tick (ATM_SAMPLE_RATE / tickRate)
// cia_count = countdown to next tick
extern uint16_t cia;
extern uint16_t cia_count;

class ATMsynth {
  public:
    ATMsynth() {};
    void play(const uint8_t *song);
    void playPause();
    void stop();
    void muteChannel(uint8_t ch);
    void unMuteChannel(uint8_t ch);
};

// oscillator structure
typedef struct {
  uint8_t  vol;
  uint16_t freq;
  uint16_t phase;
} osc_t;

typedef osc_t Oscillator;

extern osc_t osc[4];

uint16_t read_vle(const uint8_t **pp);

void ATM_playroutine();

#endif
