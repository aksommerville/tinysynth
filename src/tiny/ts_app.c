#include "base/tinysynth_platform.h"
#include <string.h>
#include <stdarg.h>
#include "common/synth/synth.h"

// Must match src/tiny/font.y8
#define GLYPHW 6
#define GLYPHH 8
#define CONCOLC (96/GLYPHW)
#define CONROWC (64/GLYPHH)

static uint8_t fb[96*64]={0};
static uint8_t console[CONCOLC*CONROWC]={0};

static uint8_t pvinput=0;

// (intake) will never contain more than a MIDI event, ie 3 bytes.
static uint8_t intake[3];
static uint8_t intakec=0;

static void console_scroll() {
  memmove(console,console+CONCOLC,CONCOLC*(CONROWC-1));
  memset(console+CONCOLC*(CONROWC-1),0,CONCOLC);
}

static void console_print(const char *fmt,...) {
  console_scroll();
  uint8_t dstx=0;
  uint8_t *dstp=console+CONCOLC*(CONROWC-1);
  va_list vargs;
  va_start(vargs,fmt);
  for (;*fmt;fmt++) {
    if (*fmt=='%') {
      fmt++;
      if (!*fmt) break;
      switch (*fmt) {
        case 'B': {
            int v=va_arg(vargs,int);
            if (dstx<CONCOLC) *dstp="0123456789abcdef"[(v>>4)&15]; dstp++; dstx++;
            if (dstx<CONCOLC) *dstp="0123456789abcdef"[v&15]; dstp++; dstx++;
          } break;
        case 'S': {
            int v=va_arg(vargs,int);
            if (dstx<CONCOLC) *dstp="0123456789abcdef"[(v>>12)&15]; dstp++; dstx++;
            if (dstx<CONCOLC) *dstp="0123456789abcdef"[(v>>8)&15]; dstp++; dstx++;
            if (dstx<CONCOLC) *dstp="0123456789abcdef"[(v>>4)&15]; dstp++; dstx++;
            if (dstx<CONCOLC) *dstp="0123456789abcdef"[v&15]; dstp++; dstx++;
          } break;
      }
    } else if (*fmt==0x0a) {
      console_scroll();
      dstx=0;
      dstp=console+CONCOLC*(CONROWC-1);
    } else {
      if (dstx<CONCOLC) *dstp=*fmt;
      dstx++;
      dstp++;
    }
  }
}

void setup() {
  tinysynth_platform_init();
  synth_init(AUDIO_RATE);
  console_print("Ready.");
}

static void draw_glyph(uint8_t *dst,uint8_t tileid,uint8_t color) {
  uint16_t srcx=(tileid&15)*GLYPHW;
  uint16_t srcy=(tileid>>4)*GLYPHH;
  const uint8_t *srcrow=font+srcy*96+srcx;
  uint8_t yi=GLYPHH;
  for (;yi-->0;dst+=96,srcrow+=96) {
    const uint8_t *srcp=srcrow;
    uint8_t *dstp=dst;
    uint8_t xi=GLYPHW;
    for (;xi-->0;dstp++,srcp++) {
      if (*srcp) *dstp=color;
    }
  }
}

static void render() {
  memset(fb,0,sizeof(fb));
  const uint8_t *dstrow=fb;
  const uint16_t rowstride=96*GLYPHH;
  const uint8_t *src=console;
  uint8_t rowi=CONROWC;
  for (;rowi-->0;dstrow+=rowstride) {
    uint8_t *dstp=dstrow;
    uint8_t coli=CONCOLC;
    for (;coli-->0;src++,dstp+=GLYPHW) {
      if ((*src>0x20)&&(*src<0x80)) {
        draw_glyph(dstp,(*src)-0x20,0xff);
      }
    }
  }
}

#define synth_event_realtime(v) { console_print("sys %B",v); synth_event_realtime(v); }
#define synth_event_note_off(chid,noteid,velocity) { console_print("off %B %B %B",chid,noteid,velocity); synth_event_note_off(chid,noteid,velocity); }
#define synth_event_note_on(chid,noteid,velocity) { console_print("on %B %B %B",chid,noteid,velocity); synth_event_note_on(chid,noteid,velocity); }
#define synth_event_note_adjust(chid,noteid,velocity) { console_print("adj %B %B %B",chid,noteid,velocity); synth_event_note_adjust(chid,noteid,velocity); }
#define synth_event_control(chid,k,v) { console_print("ctl %B %B=%B",chid,k,v); synth_event_control(chid,k,v); }
#define synth_event_program(chid,pid) { console_print("pgm %B %B",chid,pid); synth_event_program(chid,pid); }
#define synth_event_pressure(chid,v) { console_print("pres %B %B",chid,v); synth_event_pressure(chid,v); }
#define synth_event_wheel(chid,v) { console_print("wheel %B %S",chid,v); synth_event_wheel(chid,v); }

static void receive_byte(uint8_t src) {

  // Top four bits set, it's a system event, interrupts regular events.
  // ...well technically, it's 0xf8..0xff the real-time ones. 0xf0..0xf7 are different.
  if (src>=0xf0) {
    synth_event_realtime(src);
    return;
  }

  // High bit set, it's the start of an event. Drop anything we had before.
  if (src&0x80) {
    intakec=0;
  }
  
  // Add to intake.
  intake[intakec++]=src;
  
  // Check for ready events.
  switch (intake[0]&0xf0) {
    case 0x80: if (intakec>=3) { synth_event_note_off(intake[0]&15,intake[1],intake[2]); intakec=0; } break;
    case 0x90: if (intakec>=3) { synth_event_note_on(intake[0]&15,intake[1],intake[2]); intakec=0; } break;
    case 0xa0: if (intakec>=3) { synth_event_note_adjust(intake[0]&15,intake[1],intake[2]); intakec=0; } break;
    case 0xb0: if (intakec>=3) { synth_event_control(intake[0]&15,intake[1],intake[2]); intakec=0; } break;
    case 0xc0: if (intakec>=2) { synth_event_program(intake[0]&15,intake[1]); intakec=0; } break;
    case 0xd0: if (intakec>=2) { synth_event_pressure(intake[0]&15,intake[1]); intakec=0; } break;
    case 0xe0: if (intakec>=3) { synth_event_wheel(intake[0]&15,intake[1]|(intake[2]<<7)); intakec=0; } break;
    default: intakec=0;
  }
}

void loop() {

  int16_t *buf=0;
  uint16_t bufc=tinysynth_platform_get_audio_buffer(&buf);
  synth_update(buf,bufc);
  tinysynth_platform_filled_audio_buffer(buf,bufc);

  uint8_t input=tinysynth_platform_read_input();
  if (input!=pvinput) {
    console_print("input %B",input);
    pvinput=input;
  }
  
  int src;
  while ((src=usb_read_byte())>=0) receive_byte(src);

  render();
  tinysynth_platform_send_framebuffer(fb);
}
