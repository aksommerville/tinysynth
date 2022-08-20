#ifndef TINYSYNTH_PLATFORM_H
#define TINYSYNTH_PLATFORM_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>

#define AUDIO_RATE 22050

#define TS_BUTTON_LEFT 0x01
#define TS_BUTTON_RIGHT 0x02
#define TS_BUTTON_UP 0x04
#define TS_BUTTON_DOWN 0x08
#define TS_BUTTON_A 0x10
#define TS_BUTTON_B 0x20

void tinysynth_platform_init();
uint8_t tinysynth_platform_read_input();
void tinysynth_platform_send_framebuffer(const void *fb); // 96x64xbgr332

// Returns a pointer to a buffer, and its length, that you should fill.
// Call tinysynth_platform_filled_audio_buffer() after you've done so.
uint16_t tinysynth_platform_get_audio_buffer(int16_t **dstpp);
void tinysynth_platform_filled_audio_buffer(int16_t *v,uint16_t c);

extern uint32_t tinysynth_underflow;

void usb_begin();
void usb_send(const void *v,int c);
int usb_read(void *dst,int dsta);
int usb_read_byte();

/* 96x48xy8 (all 0 or 255).
 * 16x6 tiles of 6x8 pixels each.
 * No extra spacing necessary, pack the tiles tight.
 */
extern const uint8_t font[];

#ifdef __cplusplus
  }
#endif
#endif
