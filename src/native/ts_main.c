#include "ts_native.h"
#include <signal.h>

struct ts ts={0};

/* Cleanup.
 */
 
static void ts_cleanup(struct ts *ts) {
  io_broadcast(ts->io,"\xff",1);
  io_del(ts->io);
  alsa_del(ts->alsa);
  memset(ts,0,sizeof(struct ts));
}

/* Signal handler.
 */

static void rcvsig(int sigid) {
  switch (sigid) {
    case SIGINT: if (++(ts.sigc)>=3) {
        fprintf(stderr,"Too many unprocessed signals.\n");
        io_broadcast(ts.io,"\xff",1);
        exit(1);
      } break;
  }
}

/* Receive MIDI.
 * We are not distinguishing the various input channels, and assuming that Running Status and Note On Zero are not used.
 * And assuming that input chunks will break on event boundaries.
 * We assume a lot.
 * Update 2022-08-30: Piping events through Chrome, it does use Running Status. Clean this up.
 */
 
static int ts_receive_midi(const uint8_t *src,int srcc) {

  if (io_broadcast(ts.io,src,srcc)<0) return -1;
  
  // Let's assume that inputs always break on event boundaries and Running Status will not be used.
  if (alsa_lock(ts.alsa)>=0) {
    const uint8_t *SRC=src;
    int srcp=0;
    while (srcp<srcc) {
      struct ts_midi_event event;
      int err=ts_midi_stream_read(&event,&ts.midi_stream_reader,SRC+srcp,srcc-srcp);
      if (err<=0) { srcp++; continue; } // error. skip one byte and hope we eventually sync up
      srcp+=err;
      //fprintf(stderr,"PARSED %02x %02x %02x %02x\n",event.opcode,event.chid,event.a,event.b);
      switch (event.opcode) {
        case 0x80: synth_event_note_off(event.chid,event.a,event.b); break;
        case 0x90: synth_event_note_on(event.chid,event.a,event.b); break;
        case 0xa0: synth_event_note_adjust(event.chid,event.a,event.b); break;
        case 0xb0: synth_event_control(event.chid,event.a,event.b); break;
        case 0xc0: synth_event_program(event.chid,event.a); break;
        case 0xd0: synth_event_pressure(event.chid,event.a); break;
        case 0xe0: synth_event_wheel(event.chid,event.a|(event.b<<7)); break;
        case 0xf8: case 0xf9: case 0xfa: case 0xfb:
        case 0xfc: case 0xfd: case 0xfe: case 0xff: synth_event_realtime(event.opcode); break;
      }
    }
    alsa_unlock(ts.alsa);
  }
  
  return 0;
}

/* Callbacks from io.
 */

static int cb_change(struct io *io,int devid,char direction,int state) {
  fprintf(stderr,"%s %s device %d\n",state?"Connected":"Disconnected",(direction=='r')?"input":"output",devid);
  return 0;
}

static int cb_input(struct io *io,int devid,const void *src,int srcc) {
  //fprintf(stderr,"%s devid=%d srcc=%d\n",__func__,devid,srcc);
  return ts_receive_midi(src,srcc);
}

/* Callbacks from alsa.
 */
 
static int cb_pcm_out(int16_t *v,int c,struct alsa *alsa) {
  synth_update(v,c);
  return 0;
}

static int cb_midi_in(const void *src,int srcc,struct alsa *alsa) {
  /*
  fprintf(stderr,"%s [%d]",__func__,srcc);
  int i=0; for (;i<srcc;i++) fprintf(stderr," %02x",((uint8_t*)src)[i]);
  fprintf(stderr,"\n");
  /**/
  return ts_receive_midi(src,srcc);
}

/* --help
 */
 
static void print_help(const char *exename) {
  if (!exename||!exename[0]) exename="tinysynth";
  fprintf(stderr,"Usage: %s [OPTIONS]\n",exename);
  fprintf(stderr,"Default behavior is to listen to MIDI hardware and broadcast to all connected Tinies.\n");
  fprintf(stderr,"If you're using a DAW: --no-midi-devices --promiscuous\n");
  fprintf(stderr,"If you want sound locally instead of on the Tiny: --no-tiny-devices --audio\n");
  fprintf(stderr,
    "OPTIONS:\n"
    "  --help               Print this message.\n"
    "  --no-midi-devices    Don't open OSS MIDI devices directly.\n"
    "  --no-tiny-output     Don't broadcast to connected serial devices.\n"
    "  --promiscuous        Create an ALSA virtual MIDI receiver.\n"
    "  --audio              Run the synth locally and output direct to ALSA.\n"
  );
}

/* Main.
 */

int main(int argc,char **argv) {
  fprintf(stderr,"%s: Starting up. SIGINT to quit.\n",argv[0]);
  signal(SIGINT,rcvsig);
  
  struct io_delegate delegate={
    .use_oss_midi=1,
    .use_tiny=1,
    .change=cb_change,
    .input=cb_input,
  };
  int promiscuous=0,audio=0;
  int pollto=1000;
  
  int argi=1; for (;argi<argc;argi++) {
    const char *arg=argv[argi];
    if (!strcmp(arg,"--help")) {
      print_help(argv[0]);
      return 0;
    } else if (!strcmp(arg,"--no-midi-devices")) {
      delegate.use_oss_midi=0;
    } else if (!strcmp(arg,"--no-tiny-output")) {
      delegate.use_tiny=0;
    } else if (!strcmp(arg,"--promiscuous")) {
      promiscuous=1;
    } else if (!strcmp(arg,"--audio")) {
      audio=1;
    } else {
      fprintf(stderr,"%s: Ignoring unexpected argument '%s'\n",argv[0],arg);
    }
  }
  
  if (!(ts.io=io_new(&delegate))) return 1;
  
  if (promiscuous||audio) {
    struct alsa_delegate alsa_delegate={
      .rate=22050,
      .chanc=1,
      .device=0,
      .userdata=0,
      .cb_pcm_out=cb_pcm_out,
      .cb_midi_in=cb_midi_in,
    };
    if (promiscuous) {
      alsa_delegate.cb_midi_in=cb_midi_in; // this hook is the "enable" flag
      pollto=20;
    }
    if (!(ts.alsa=alsa_new(&alsa_delegate))) {
      fprintf(stderr,"Failed to initialize ALSA client.\n");
      return 1;
    }
    if (audio) {
      synth_init(alsa_get_rate(ts.alsa));
      //synth_play_song(song_sevencircles,song_sevencircles_length,0,1);//XXX
    }
  }
  
  while (!ts.sigc) {
    if (ts.alsa&&(alsa_update(ts.alsa)<0)) {
      fprintf(stderr,"Error updating ALSA.\n");
      ts_cleanup(&ts);
      return 1;
    }
    if (io_update(ts.io,pollto)<0) {
      fprintf(stderr,"Error updating I/O bouncer.\n");
      ts_cleanup(&ts);
      return 1;
    }
  }
  
  ts_cleanup(&ts);
  fprintf(stderr,"%s: Normal exit.\n",argv[0]);
  return 0;
}
