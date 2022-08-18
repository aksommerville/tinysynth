/* synth.h
 * Interface to the synthesizer.
 * We build this for both native and tiny.
 */
 
#ifndef SYNTH_H
#define SYNTH_H

#include <stdint.h>

void synth_init(uint32_t rate);

void synth_update(int16_t *v,int c);

void synth_event_system(uint8_t e);
void synth_event_note_off(uint8_t chid,uint8_t noteid,uint8_t velocity);
void synth_event_note_on(uint8_t chid,uint8_t noteid,uint8_t velocity);
void synth_event_note_adjust(uint8_t chid,uint8_t noteid,uint8_t velocity);
void synth_event_control(uint8_t chid,uint8_t k,uint8_t v);
void synth_event_program(uint8_t chid,uint8_t pid);
void synth_event_pressure(uint8_t chid,uint8_t v);
void synth_event_wheel(uint8_t chid,uint16_t v);

#endif
