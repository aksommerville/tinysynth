#include "alsa.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <sys/poll.h>
#include <alsa/asoundlib.h>

/* Object definition.
 */

struct alsa {
  struct alsa_delegate delegate;
  int refc;

  snd_pcm_t *alsa;
  snd_pcm_hw_params_t *hwparams;
  snd_rawmidi_t *rawmidi;

  int hwbuffersize;
  int bufc; // frames
  int bufc_samples;
  int16_t *buf;

  pthread_t iothd;
  pthread_mutex_t iomtx;
  int ioabort;
  int cberror;
};

/* Delete.
 */
 
void alsa_del(struct alsa *alsa) {
  if (!alsa) return;
  if (alsa->refc-->1) return;
  
  alsa->ioabort=1;
  if (alsa->iothd&&!alsa->cberror) {
    pthread_cancel(alsa->iothd);
    pthread_join(alsa->iothd,0);
  }
  pthread_mutex_destroy(&alsa->iomtx);
  if (alsa->hwparams) snd_pcm_hw_params_free(alsa->hwparams);
  if (alsa->alsa) snd_pcm_close(alsa->alsa);
  if (alsa->buf) free(alsa->buf);
  if (alsa->rawmidi) snd_rawmidi_close(alsa->rawmidi);
  
  free(alsa);
}

/* Retain.
 */
 
int alsa_ref(struct alsa *alsa) {
  if (!alsa) return -1;
  if (alsa->refc<1) return -1;
  if (alsa->refc==INT_MAX) return -1;
  alsa->refc++;
  return 0;
}

/* I/O thread.
 */

static void *_alsa_iothd(void *dummy) {
  struct alsa *alsa=dummy;
  //pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,0);
  while (1) {
    pthread_testcancel();

    if (pthread_mutex_lock(&alsa->iomtx)) {
      alsa->cberror=1;
      return 0;
    }
    alsa->delegate.cb_pcm_out(alsa->buf,alsa->bufc_samples,alsa);
    pthread_mutex_unlock(&alsa->iomtx);
    if (alsa->ioabort) {
      fprintf(stderr,"alsa: terminate io thread due to ioabort\n");
      return 0;
    }

    int16_t *samplev=alsa->buf;
    int samplep=0,samplec=alsa->bufc;
    while (samplep<samplec) {
      pthread_testcancel();
      int err=snd_pcm_writei(alsa->alsa,samplev+samplep,samplec-samplep);
      if (alsa->ioabort) {
        fprintf(stderr,"alsa: terminate io thread due to ioabort (2)\n");
        return 0;
      }
      if (err<=0) {
        if ((err=snd_pcm_recover(alsa->alsa,err,0))<0) {
          alsa->cberror=1;
          fprintf(stderr,"alsa: terminate io thread due snd_pcm_recover failed\n");
          return 0;
        }
        break;
      }
      samplep+=err*alsa->delegate.chanc;
    }
  }
  return 0;
}

/* Init MIDI-in.
 */
 
static int alsa_midi_init(struct alsa *alsa) {
  if (snd_rawmidi_open(&alsa->rawmidi,0,"virtual",0)<0) return -1;
  return 0;
}

/* Init.
 */
 
static int _alsa_init(struct alsa *alsa) {
  
  if (!alsa->delegate.device||!alsa->delegate.device[0]) {
    alsa->delegate.device="default";
  }
  
  // Ideally, our buffer size is just under one video frame.
  // If it's larger, the visible notes will skip a little. (it's not that big a deal).
  // Experimentally at higher rates (about 40 kHz and up), we get noise and need a slighly larger buffer.
  // So targetting 30 Hz instead of 60. You can change this 30 to 60 for smoother animation, if it sounds ok great.
  int buffer_size_limit=alsa->delegate.rate/30;
  int buffer_size=256;
  while (1) {
    int next_size=buffer_size<<1;
    if (next_size>=buffer_size_limit) break;
    buffer_size=next_size;
  }

  if (
    (snd_pcm_open(&alsa->alsa,alsa->delegate.device,SND_PCM_STREAM_PLAYBACK,0)<0)||
    (snd_pcm_hw_params_malloc(&alsa->hwparams)<0)||
    (snd_pcm_hw_params_any(alsa->alsa,alsa->hwparams)<0)||
    (snd_pcm_hw_params_set_access(alsa->alsa,alsa->hwparams,SND_PCM_ACCESS_RW_INTERLEAVED)<0)||
    (snd_pcm_hw_params_set_format(alsa->alsa,alsa->hwparams,SND_PCM_FORMAT_S16)<0)||
    (snd_pcm_hw_params_set_rate_near(alsa->alsa,alsa->hwparams,&alsa->delegate.rate,0)<0)||
    (snd_pcm_hw_params_set_channels_near(alsa->alsa,alsa->hwparams,&alsa->delegate.chanc)<0)||
    (snd_pcm_hw_params_set_buffer_size(alsa->alsa,alsa->hwparams,buffer_size)<0)||
    (snd_pcm_hw_params(alsa->alsa,alsa->hwparams)<0)
  ) return -1;
  
  if (snd_pcm_nonblock(alsa->alsa,0)<0) return -1;
  if (snd_pcm_prepare(alsa->alsa)<0) return -1;

  alsa->bufc=buffer_size;
  alsa->bufc_samples=alsa->bufc*alsa->delegate.chanc;
  if (!(alsa->buf=malloc(alsa->bufc_samples*2))) return -1;

  pthread_mutexattr_t mattr;
  pthread_mutexattr_init(&mattr);
  pthread_mutexattr_settype(&mattr,PTHREAD_MUTEX_RECURSIVE);
  if (pthread_mutex_init(&alsa->iomtx,&mattr)) return -1;
  pthread_mutexattr_destroy(&mattr);
  if (pthread_create(&alsa->iothd,0,_alsa_iothd,alsa)) return -1;
  
  if (alsa->delegate.cb_midi_in) {
    if (alsa_midi_init(alsa)<0) {
      fprintf(stderr,"Failed to initialize ALSA RawMIDI. Proceeding without MIDI input.\n");
    }
  }
  
  return 0;
}

/* New.
 */
 
struct alsa *alsa_new(const struct alsa_delegate *delegate) {
  if (!delegate||!delegate->cb_pcm_out) return 0;
  
  struct alsa *alsa=calloc(1,sizeof(struct alsa));
  if (!alsa) return 0;
  
  alsa->refc=1;
  memcpy(&alsa->delegate,delegate,sizeof(struct alsa_delegate));
  
  if (_alsa_init(alsa)<0) {
    alsa_del(alsa);
    return 0;
  }
  
  return alsa;
}

/* Trivial accessors.
 */
 
int alsa_get_rate(const struct alsa *alsa) {
  if (!alsa) return 0;
  return alsa->delegate.rate;
}

int alsa_get_chanc(const struct alsa *alsa) {
  if (!alsa) return 0;
  return alsa->delegate.chanc;
}

void *alsa_get_userdata(const struct alsa *alsa) {
  if (!alsa) return 0;
  return alsa->delegate.userdata;
}
  
int alsa_get_status(const struct alsa *alsa) {
  if (!alsa) return -1;
  if (alsa->cberror) return -1;
  return 0;
}

/* Lock.
 */
 
int alsa_lock(struct alsa *alsa) {
  if (!alsa) return 0;
  if (pthread_mutex_lock(&alsa->iomtx)) return -1;
  return 0;
}

int alsa_unlock(struct alsa *alsa) {
  if (!alsa) return 0;
  if (pthread_mutex_unlock(&alsa->iomtx)) return -1;
  return 0;
}

/* Update.
 */
 
int alsa_update(struct alsa *alsa) {
  if (alsa->rawmidi) {
    int fdc=snd_rawmidi_poll_descriptors_count(alsa->rawmidi);
    if (fdc>0) {
      struct pollfd pollfd={0};
      int err=snd_rawmidi_poll_descriptors(alsa->rawmidi,&pollfd,1);
      if ((err>0)&&(poll(&pollfd,1,0)>0)) {
        char tmp[256];
        int tmpc=snd_rawmidi_read(alsa->rawmidi,tmp,sizeof(tmp));
        if (tmpc<=0) {
          fprintf(stderr,"Error reading ALSA MIDI. Closing connection. tmpc=%d errno=%d %m\n",tmpc,errno);
          snd_rawmidi_close(alsa->rawmidi);
          alsa->rawmidi=0;
        } else {
          if (alsa->delegate.cb_midi_in(tmp,tmpc,alsa)<0) return -1;
        }
      }
    }
  }
  return 0;
}
