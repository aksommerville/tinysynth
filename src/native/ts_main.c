#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdint.h>

static volatile int sigc=0;
static int fd=-1;
static int srcfd=-1;

static void rcvsig(int sigid) {
  switch (sigid) {
    case SIGINT: if (++sigc>=3) {
        fprintf(stderr,"Too many unprocessed signals.\n");
        exit(1);
      } break;
  }
}

int main(int argc,char **argv) {
  fprintf(stderr,"%s: Starting up.\n",argv[0]);
  signal(SIGINT,rcvsig);
  while (!sigc) {
  
    //TODO poll inputs and inotify
    // Doing it hackily just to check lights on.
    if (fd<0) {
      if ((fd=open("/dev/ttyACM0",O_WRONLY))>=0) {
        fprintf(stderr,"Connected to Tiny.\n");
      }
    }
    if (srcfd<0) {
      if ((srcfd=open("/dev/midi1",O_RDONLY))>=0) {
        fprintf(stderr,"Connected to MIDI.\n");
      } else {
        usleep(100000);
        continue;
      }
    }
    
    // One kind of weird thing: SIGINT doesn't interrupt my read() of /dev/midi1. Hoping it will interrupt poll() in real life.
    uint8_t buf[16];
    int bufc=read(srcfd,buf,sizeof(buf));
    if (bufc<=0) {
      fprintf(stderr,"Failed to read from MIDI, closing.\n");
      close(srcfd);
      srcfd=-1;
      continue;
    }
    if (fd<0) {
      fprintf(stderr,"Tiny not yet online, discarding %d bytes of MIDI.\n",bufc);
    } else {
      if (write(fd,buf,bufc)!=bufc) {
        fprintf(stderr,"Write failed, dropping Tiny connection.\n");
        close(fd);
        fd=-1;
      } else {
        //fprintf(stderr,"thru %d\n",bufc);
      }
    }
  }
  if (fd>=0) close(fd);
  if (srcfd>=0) close(srcfd);
  fprintf(stderr,"%s: Normal exit.\n",argv[0]);
  return 0;
}
