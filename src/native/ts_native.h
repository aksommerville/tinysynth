#ifndef TS_NATIVE_H
#define TS_NATIVE_H

#include "io.h"
#include "alsa.h"
#include "common/synth/synth.h"
#include "common/ts_midi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

extern struct ts {
  volatile int sigc;
  struct io *io;
  struct alsa *alsa;
  struct ts_midi_stream_reader midi_stream_reader;
} ts;

#endif
