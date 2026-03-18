#include "ATMlib.h"
#include "EEPROM.h"
// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------

uint8_t trackCount;
uint8_t tickRate;
const uint16_t *trackList;
const uint8_t *trackBase;
uint8_t pcm = 128;
bool half;

uint8_t ChannelActiveMute = 0b11110000;

uint16_t cia;
uint16_t cia_count;

osc_t osc[4];

const uint16_t noteTable[64] PROGMEM = {
  0,
  262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,
  523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,
  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
  4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902,
  8372, 8870, 9397,
};

// ---------------------------------------------------------------------------
// Playdate audio state
// ---------------------------------------------------------------------------

static SoundSource *atm_soundSource = nullptr;
static volatile bool atm_playing = false;
static volatile bool atm_resetting = false;

// Sample rate the Playdate audio system runs at
#define ATM_SAMPLE_RATE 44100

// ---------------------------------------------------------------------------
// Channel structure (same as original)
// ---------------------------------------------------------------------------

struct ch_t {
  const uint8_t *ptr;
  uint8_t note;

  uint16_t stackPointer[7];
  uint8_t stackCounter[7];
  uint8_t stackTrack[7];
  uint8_t stackIndex;
  uint8_t repeatPoint;

  uint16_t delay;
  uint8_t counter;
  uint8_t track;

  uint16_t freq;
  uint8_t vol;

  int8_t volFreSlide;
  uint8_t volFreConfig;
  uint8_t volFreCount;

  uint8_t arpNotes;
  uint8_t arpTiming;
  uint8_t arpCount;

  uint8_t reConfig;
  uint8_t reCount;

  int8_t transConfig;

  uint8_t treviDepth;
  uint8_t treviConfig;
  uint8_t treviCount;

  int8_t glisConfig;
  uint8_t glisCount;
};

ch_t channel[4];

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

uint16_t read_vle(const uint8_t **pp) {
  uint16_t q = 0;
  uint8_t d;
  do {
    q <<= 7;
    d = pgm_read_byte(*pp++);
    q |= (d & 0x7F);
  } while (d & 0x80);
  return q;
}

static inline const uint8_t *getTrackPointer(uint8_t track) {
  if (track >= trackCount) return trackBase; // safety: return start of first track
  return trackBase + pgm_read_word(&trackList[track]);
}

// ---------------------------------------------------------------------------
// ATM_playroutine — unchanged logic from original
// ---------------------------------------------------------------------------

void ATM_playroutine() {
  ch_t *ch;

  for (uint8_t n = 0; n < 4; n++) {
    ch = &channel[n];

    // Noise retriggering
    if (ch->reConfig) {
      if (ch->reCount >= (ch->reConfig & 0x03)) {
        osc[n].freq = pgm_read_word(&noteTable[ch->reConfig >> 2]);
        ch->reCount = 0;
      } else ch->reCount++;
    }

    // Apply Glissando
    if (ch->glisConfig) {
      if (ch->glisCount >= (ch->glisConfig & 0x7F)) {
        if (ch->glisConfig & 0x80) ch->note -= 1;
        else ch->note += 1;
        if (ch->note < 1) ch->note = 1;
        else if (ch->note > 63) ch->note = 63;
        ch->freq = pgm_read_word(&noteTable[ch->note]);
        ch->glisCount = 0;
      } else ch->glisCount++;
    }

    // Apply volume/frequency slides
    if (ch->volFreSlide) {
      if (!ch->volFreCount) {
        int16_t vf = ((ch->volFreConfig & 0x40) ? ch->freq : ch->vol);
        vf += (ch->volFreSlide);
        if (!(ch->volFreConfig & 0x80)) {
          if (vf < 0) vf = 0;
          else if (ch->volFreConfig & 0x40) { if (vf > 9397) vf = 9397; }
          else { if (vf > 63) vf = 63; }
        }
        (ch->volFreConfig & 0x40) ? ch->freq = vf : ch->vol = vf;
      }
      if (ch->volFreCount++ >= (ch->volFreConfig & 0x3F)) ch->volFreCount = 0;
    }

    // Apply Arpeggio or Note Cut
    if (ch->arpNotes && ch->note) {
      if ((ch->arpCount & 0x1F) < (ch->arpTiming & 0x1F)) ch->arpCount++;
      else {
        if ((ch->arpCount & 0xE0) == 0x00) ch->arpCount = 0x20;
        else if ((ch->arpCount & 0xE0) == 0x20 && !(ch->arpTiming & 0x40) && (ch->arpNotes != 0xFF)) ch->arpCount = 0x40;
        else ch->arpCount = 0x00;
        uint8_t arpNote = ch->note;
        if ((ch->arpCount & 0xE0) != 0x00) {
          if (ch->arpNotes == 0xFF) arpNote = 0;
          else arpNote += (ch->arpNotes >> 4);
        }
        if ((ch->arpCount & 0xE0) == 0x40) arpNote += (ch->arpNotes & 0x0F);
        ch->freq = pgm_read_word(&noteTable[arpNote + ch->transConfig]);
      }
    }

    // Apply Tremolo or Vibrato
    if (ch->treviDepth) {
      int16_t vt = ((ch->treviConfig & 0x40) ? ch->freq : ch->vol);
      vt = (ch->treviCount & 0x80) ? (vt + ch->treviDepth) : (vt - ch->treviDepth);
      if (vt < 0) vt = 0;
      else if (ch->treviConfig & 0x40) { if (vt > 9397) vt = 9397; }
      else { if (vt > 63) vt = 63; }
      (ch->treviConfig & 0x40) ? ch->freq = vt : ch->vol = vt;
      if ((ch->treviCount & 0x1F) < (ch->treviConfig & 0x1F)) ch->treviCount++;
      else {
        if (ch->treviCount & 0x80) ch->treviCount = 0;
        else ch->treviCount = 0x80;
      }
    }

    if (ch->delay) {
      if (ch->delay != 0xFFFF) ch->delay--;
    } else {
      do {
        uint8_t cmd = pgm_read_byte(ch->ptr++);
        if (cmd < 64) {
          if ((ch->note = cmd)) ch->note += ch->transConfig;
          ch->freq = pgm_read_word(&noteTable[ch->note]);
          if (!ch->volFreConfig) ch->vol = ch->reCount;
          if (ch->arpTiming & 0x20) ch->arpCount = 0;
        } else if (cmd < 160) {
          switch (cmd - 64) {
            case 0:
              ch->vol = pgm_read_byte(ch->ptr++);
              ch->reCount = ch->vol;
              break;
            case 1: case 4:
              ch->volFreSlide = pgm_read_byte(ch->ptr++);
              ch->volFreConfig = (cmd - 64) == 1 ? 0x00 : 0x40;
              break;
            case 2: case 5:
              ch->volFreSlide = pgm_read_byte(ch->ptr++);
              ch->volFreConfig = pgm_read_byte(ch->ptr++);
              break;
            case 3: case 6:
              ch->volFreSlide = 0;
              break;
            case 7:
              ch->arpNotes = pgm_read_byte(ch->ptr++);
              ch->arpTiming = pgm_read_byte(ch->ptr++);
              break;
            case 8:
              ch->arpNotes = 0;
              break;
            case 9:
              ch->reConfig = pgm_read_byte(ch->ptr++);
              break;
            case 10:
              ch->reConfig = 0;
              break;
            case 11:
              ch->transConfig += (char)pgm_read_byte(ch->ptr++);
              break;
            case 12:
              ch->transConfig = pgm_read_byte(ch->ptr++);
              break;
            case 13:
              ch->transConfig = 0;
              break;
            case 14: case 16:
              ch->treviDepth = pgm_read_byte(ch->ptr++);
              ch->treviConfig = pgm_read_byte(ch->ptr++) + ((cmd - 64) == 14 ? 0x00 : 0x40);
              break;
            case 15: case 17:
              ch->treviDepth = 0;
              break;
            case 18:
              ch->glisConfig = pgm_read_byte(ch->ptr++);
              break;
            case 19:
              ch->glisConfig = 0;
              break;
            case 20:
              ch->arpNotes = 0xFF;
              ch->arpTiming = pgm_read_byte(ch->ptr++);
              break;
            case 21:
              ch->arpNotes = 0;
              break;
            case 92:
              tickRate += pgm_read_byte(ch->ptr++);
              if (tickRate == 0) tickRate = 1;
              cia = ATM_SAMPLE_RATE / tickRate;
              break;
            case 93:
              tickRate = pgm_read_byte(ch->ptr++);
              if (tickRate == 0) tickRate = 1;
              cia = ATM_SAMPLE_RATE / tickRate;
              break;
            case 94:
              for (uint8_t i = 0; i < 4; i++) channel[i].repeatPoint = pgm_read_byte(ch->ptr++);
              break;
            case 95:
              ChannelActiveMute = ChannelActiveMute ^ (1 << (n + 4));
              ch->vol = 0;
              ch->delay = 0xFFFF;
              break;
          }
        } else if (cmd < 224) {
          ch->delay = cmd - 159;
        } else if (cmd == 224) {
          ch->delay = read_vle(&ch->ptr) + 65;
        } else if (cmd < 252) {
          // reserved
        } else if (cmd == 252 || cmd == 253) {
          uint8_t new_counter = cmd == 252 ? 0 : pgm_read_byte(ch->ptr++);
          uint8_t new_track = pgm_read_byte(ch->ptr++);
          if (ch->stackIndex < 7) { // bounds check
            ch->stackCounter[ch->stackIndex] = ch->counter;
            ch->stackTrack[ch->stackIndex] = ch->track;
            ch->stackPointer[ch->stackIndex] = ch->ptr - trackBase;
            ch->stackIndex++;
          }
          ch->track = new_track;
          ch->counter = new_counter;
          ch->ptr = getTrackPointer(ch->track);
        } else if (cmd == 254) {
          if (ch->counter > 0) {
            ch->counter--;
            ch->ptr = getTrackPointer(ch->track);
          } else if (ch->stackIndex > 0) {
            ch->stackIndex--;
            ch->ptr = ch->stackPointer[ch->stackIndex] + trackBase;
            ch->counter = ch->stackCounter[ch->stackIndex];
            ch->track = ch->stackTrack[ch->stackIndex];
          } else {
            // stack empty, stop channel
            ch->delay = 0xFFFF;
          }
        } else if (cmd == 255) {
          ch->ptr += read_vle(&ch->ptr);
        }
      } while (ch->delay == 0);

      if (ch->delay != 0xFFFF) ch->delay--;
    }

    if (!(ChannelActiveMute & (1 << n))) {
      if (n == 3) {
        osc[n].vol = ch->vol >> 1;
      } else {
        osc[n].freq = ch->freq;
        osc[n].vol = ch->vol;
      }
    }

    if (!(ChannelActiveMute & 0xF0)) {
      uint8_t repeatSong = 0;
      for (uint8_t j = 0; j < 4; j++) repeatSong += channel[j].repeatPoint;
      if (repeatSong && !atm_resetting) {
        for (uint8_t k = 0; k < 4; k++) {
          channel[k].ptr = getTrackPointer(channel[k].repeatPoint);
          channel[k].delay = 1;
        }
        ChannelActiveMute = 0b11110000;
      } else {
        memset(channel, 0, sizeof(channel));
        atm_playing = false;
      }
      return; // MUST return: remaining channels in the loop now have ptr=0
    }
  }
}

// ---------------------------------------------------------------------------
// Playdate audio source callback
// Mixes 4 oscillators into stereo int16 samples at ATM_SAMPLE_RATE
//
// Waveforms (derived from ISR assembly):
//   ch0: square  — bit7 of phase_hi XOR (bit7 AND bit6): actually pulse
//   ch1: square  — bit7 of phase_hi
//   ch2: triangle — abs(phase_hi) scaled by vol
//   ch3: LFSR noise
//
// The original ran at 62500Hz. We run at 44100Hz.
// cia = ATM_SAMPLE_RATE / tickRate so timing stays correct.
// ---------------------------------------------------------------------------

extern "C" {
int audioCallback(void *context, int16_t *left, int16_t *right, int len);
}

int audioCallback(void *context, int16_t *left, int16_t *right, int len) {
  int sound_on = 0;
  EEPROM.get(2, sound_on);
  if (!atm_playing || !sound_on) {
    memset(left, 0, len * sizeof(int16_t));
    return 1;
  }

  static int logCount = 0;
  if (logCount++ < 3) pd->system->logToConsole("ATMlib: audioCallback called, len=%d", len);

  // Precompute phase increments (scaled for 44100Hz sample rate)
  uint16_t inc0 = (uint16_t)((uint32_t)osc[0].freq * 65536 / ATM_SAMPLE_RATE);
  uint16_t inc1 = (uint16_t)((uint32_t)osc[1].freq * 65536 / ATM_SAMPLE_RATE);
  uint16_t inc2 = (uint16_t)((uint32_t)osc[2].freq * 65536 / ATM_SAMPLE_RATE);

  for (int i = 0; i < len; i++) {
    // Advance cia counter and call playroutine at tick rate
    cia_count--;
    if (cia_count == 0) {
      cia_count = (cia > 0) ? cia : 1;
      ATM_playroutine();
      // Recompute increments after playroutine may have changed frequencies
      inc0 = (uint16_t)((uint32_t)osc[0].freq * 65536 / ATM_SAMPLE_RATE);
      inc1 = (uint16_t)((uint32_t)osc[1].freq * 65536 / ATM_SAMPLE_RATE);
      inc2 = (uint16_t)((uint32_t)osc[2].freq * 65536 / ATM_SAMPLE_RATE);
    }

    // Advance oscillator phases
    osc[0].phase += inc0;
    osc[1].phase += inc1;
    osc[2].phase += inc2;
    // Channel 3: LFSR — shift and XOR bits 7 and 6 of high byte
    uint16_t lfsr = osc[3].freq;
    uint8_t feedback = ((lfsr >> 7) ^ (lfsr >> 6)) & 1;
    lfsr = (lfsr << 1) | feedback;
    osc[3].freq = lfsr;

    // Mix channels
    int32_t mix = 0;

    // Channel 0: pulse wave — high when bit7 AND bit6 of phase_hi both set (from asm)
    {
      uint8_t phi = osc[0].phase >> 8;
      int8_t sample = ((phi & 0x80) && (phi & 0x40)) ? (int8_t)osc[0].vol : -(int8_t)osc[0].vol;
      mix += sample;
    }

    // Channel 1: square wave — high when bit7 of phase_hi set
    {
      uint8_t phi = osc[1].phase >> 8;
      int8_t sample = (phi & 0x80) ? (int8_t)osc[1].vol : -(int8_t)osc[1].vol;
      mix += sample;
    }

    // Channel 2: triangle wave — abs(phase_hi) scaled
    {
      uint8_t phi = osc[2].phase >> 8;
      // triangle: fold at midpoint
      uint8_t tri = (phi & 0x80) ? ~phi : phi;
      tri = (tri << 1); // scale to 0..254
      int32_t sample = ((int32_t)(int8_t)(tri - 128) * osc[2].vol) >> 6;
      mix += sample;
    }

    // Channel 3: noise — bit0 of LFSR, scaled by vol
    {
      int8_t sample = (osc[3].freq & 1) ? (int8_t)(osc[3].vol >> 1) : -(int8_t)(osc[3].vol >> 1);
      mix += sample;
    }

    // Clamp to int16
    if (mix > 32767) mix = 32767;
    if (mix < -32768) mix = -32768;

    left[i] = (int16_t)(mix * 128);
    if (right) right[i] = left[i];
  }

  return 1;
}

// ---------------------------------------------------------------------------
// ATMsynth methods
// ---------------------------------------------------------------------------

void ATMsynth::play(const uint8_t *song) {
  atm_resetting = true;
  atm_playing = false;

  memset(channel, 0, sizeof(channel));
  ChannelActiveMute = 0b11110000;

  tickRate = 25;
  cia = ATM_SAMPLE_RATE / tickRate;
  cia_count = 1;

  osc[3].freq = 0x0001;
  channel[3].freq = 0x0001;

  trackCount = pgm_read_byte(song++);
  trackList = (uint16_t*)song;
  trackBase = (song += (trackCount << 1)) + 4;

  for (unsigned n = 0; n < 4; n++) {
    channel[n].ptr = getTrackPointer(pgm_read_byte(song++));
  }

  if (atm_soundSource == nullptr) {
    atm_soundSource = pd->sound->addSource(audioCallback, nullptr, 0);
    pd->system->logToConsole("ATMlib: registered audio source: %p", atm_soundSource);
  }

  atm_resetting = false;
  atm_playing = true;
  pd->system->logToConsole("ATMlib: play() called, atm_playing=true");
}

void ATMsynth::stop() {
  atm_playing = false;
  memset(channel, 0, sizeof(channel));
  ChannelActiveMute = 0b11110000;
  memset(osc, 0, sizeof(osc));
}

void ATMsynth::playPause() {
  atm_playing = !atm_playing;
}

void ATMsynth::muteChannel(uint8_t ch) {
  ChannelActiveMute |= (1 << ch);
}

void ATMsynth::unMuteChannel(uint8_t ch) {
  ChannelActiveMute &= ~(1 << ch);
}