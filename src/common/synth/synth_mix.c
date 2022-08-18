#include "synth_internal.h"

#define envstage voice->bv[0]
#define releasing voice->bv[1]
#define envc voice->lv[0]
#define envd voice->lv[1]
#define level voice->lv[2]
#define decc voice->lv[3]
#define susl voice->lv[4]
#define rlsc voice->lv[5]
#define lfop voice->lv[6]
#define wave0lv voice->pv[0]
#define wave0 ((int16_t*)voice->pv[0])
#define wave1lv voice->pv[1]
#define wave1 ((int16_t*)voice->pv[1])
#define chanlv voice->pv[2]
#define chan ((struct synth_channel*)voice->pv[2])

/* Update.
 */
 
void synth_mix_update(int16_t *v,int c,struct synth_voice *voice) {
  for (;c-->0;v++) {
  
    voice->p+=voice->rate;
    
    uint32_t p=voice->p>>SYNTH_WAVE_P_SHIFT;
    int32_t sample;
    
    /* Manual mix via mod wheel. Works good, if you're into that. *
    if (chan->mod<=0) sample=wave0[p];
    else if (chan->mod>=0x7f) sample=wave1[p];
    else sample=(wave0[p]*(0x7f-chan->mod)+wave1[p]*chan->mod)>>7;
    /**/
    
    /* LFO mix. Not sure I got this right but even if so, it would be a per-voice LFO. Not ideal. *
    lfop+=10000;//TODO lfo rate
    uint8_t mix=(wave0[((uint32_t)lfop)>>SYNTH_WAVE_P_SHIFT]>>8)+128;//TODO lfo wave
    sample=(wave0[p]*(0xff-mix)+wave1[p]*mix)>>8;
    /**/
    
    /* Linear mix at attack only. */
    if (lfop<0xffff) lfop+=10;
    uint8_t mix=(lfop>=0xffff)?0xff:(lfop>>8);
    sample=(wave0[p]*(0xff-mix)+wave1[p]*mix)>>8;
    
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
 
void synth_mix_release(struct synth_voice *voice) {
  releasing=1;
}

/* Setup.
 */
 
void synth_mix_setup(struct synth_voice *voice,uint8_t noteid,uint8_t velocity,struct synth_channel *channel) {
  voice->update=synth_mix_update;
  voice->release=synth_mix_release;
  voice->adjust=0;
  wave0lv=(void*)synth_wave_1;
  wave1lv=(void*)synth_wave_0;
  envstage=0;
  level=0;
  releasing=0;
  chanlv=channel;
  lfop=0;
  
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
