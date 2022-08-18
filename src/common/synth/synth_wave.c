#include "synth_internal.h"

#define envstage voice->bv[0]
#define releasing voice->bv[1]
#define envc voice->lv[0]
#define envd voice->lv[1]
#define level voice->lv[2]
#define decc voice->lv[3]
#define susl voice->lv[4]
#define rlsc voice->lv[5]
#define wavelv voice->pv[0]
#define wave ((int16_t*)voice->pv[0])

/* Update.
 */
 
void synth_wave_update(int16_t *v,int c,struct synth_voice *voice) {
  for (;c-->0;v++) {
  
    voice->p+=voice->rate;
    
    int32_t sample=wave[voice->p>>SYNTH_WAVE_P_SHIFT];
    
    if (envc>0) {
      envc--;
      level+=envd;
    } else switch (envstage) {
      case 0: {
          envstage=1;
          envc=decc;
          envd=(susl-level)/envc;
        } break;
      case 1: {
          envstage=2;
          envc=100;
          envd=0;
        } break;
      case 2: {
          if (releasing) {
            envstage=3;
            envc=rlsc;
            envd=-level/envc;
          } else {
            envc=100;
          }
        } break;
      case 3: {
          voice->update=0;
          return;
        } break;
    }
    
    sample=(sample*(level>>16))>>15;
    
    (*v)+=sample;
  }
}

/* Release.
 */
 
void synth_wave_release(struct synth_voice *voice) {
  releasing=1;
}

/* Setup.
 */
 
void synth_wave_setup(struct synth_voice *voice,uint8_t noteid,uint8_t velocity,struct synth_channel *channel) {
  voice->update=synth_wave_update;
  voice->release=synth_wave_release;
  voice->adjust=0;
  wavelv=(void*)synth_wave_0;
  envstage=0;
  level=0;
  releasing=0;
  
  int32_t attack_time=(channel->atktlo*(0x7f-velocity)+channel->atkthi*velocity)>>7;
  int32_t release_time=(channel->rlstlo*(0x7f-velocity)+channel->rlsthi*velocity)>>7;
  int32_t attack_level=channel->volume<<23;
  int32_t decay_time=attack_time*2;
  int32_t sustain_level=(attack_level>>7)*channel->sustainlevel;
  
  envc=attack_time;
  envd=attack_level/envc;
  decc=decay_time;
  susl=sustain_level;
  rlsc=release_time;
}
