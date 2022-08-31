/* ts_midi.h
 */

#ifndef TS_MIDI_H
#define TS_MIDI_H

#include <stdint.h>

#define TS_MIDI_OPCODE_INVALID     0x00
#define TS_MIDI_OPCODE_SYSEX       0xf0
// 80,90,a0,b0,c0,d0,f8..ff per spec

struct ts_midi_event {
  uint8_t opcode;
  uint8_t chid;
  uint8_t a;
  uint8_t b;
  const uint8_t *v;
  int c;
};

struct ts_midi_stream_reader {
  uint8_t status;
};

/* Events must not be split over multiple chunks.
 * We'll return zero if you provide something incomplete; retaining the unread bit is your problem.
 * We do not allow interruption by realtime events, but do allow them between other events.
 * Can return <0.
 */
int ts_midi_stream_read(
  struct ts_midi_event *event,
  struct ts_midi_stream_reader *reader,
  const void *src,int srcc
);

#endif
