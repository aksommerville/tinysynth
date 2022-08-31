#include "ts_midi.h"

/* Read from stream.
 */
 
int ts_midi_stream_read(
  struct ts_midi_event *event,
  struct ts_midi_stream_reader *reader,
  const void *src,int srcc
) {
  if (!event||!reader) return -1;
  if (srcc<1) return 0;
  if (!src) return -1;
  const uint8_t *SRC=src;
  int srcp=0;
  
  // Get the Status byte.
  if (SRC[srcp]&0x80) reader->status=SRC[srcp++];
  else if (reader->status&0x80) ;
  else return -1;
  
  // Initial guess (we'll amend for Sysex and Realtime).
  event->opcode=reader->status&0xf0;
  event->chid=reader->status&0x0f;
  event->v=0;
  event->c=0;
  
  switch (event->opcode) {
    #define AB { if (srcp>srcc-2) return 0; event->a=SRC[srcp++]; event->b=SRC[srcp++]; }
    #define A { if (srcp>=srcc) return 0; event->a=SRC[srcp++]; event->b=0; }
    case 0x80: AB return srcp;
    case 0x90: AB if (!event->b) { event->opcode=0x80; event->b=0x40; } return srcp;
    case 0xa0: AB return srcp;
    case 0xb0: AB return srcp;
    case 0xc0: A return srcp;
    case 0xd0: A return srcp;
    case 0xe0: AB return srcp;
    case 0xf0: {
        event->opcode=reader->status;
        reader->status=0;
        event->a=0;
        event->b=0;
        switch (event->opcode) {
          case 0xf0: {
              event->v=SRC+srcp;
              while (srcp<srcc) {
                if (SRC[srcp++]==0xf7) return srcp;
                event->c++;
              }
              return 0;
            }
          case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7: return -1;
          default: return 1; // Realtime
        }
      } return srcp;
  }
  return -1;
}
