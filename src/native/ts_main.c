#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdint.h>

static volatile int sigc=0;
static struct io *io=0;

static void rcvsig(int sigid) {
  switch (sigid) {
    case SIGINT: if (++sigc>=3) {
        fprintf(stderr,"Too many unprocessed signals.\n");
        io_broadcast(io,"\xff",1);
        exit(1);
      } break;
  }
}

static int cb_change(struct io *io,int devid,char direction,int state) {
  fprintf(stderr,"%s %s device %d\n",state?"Connected":"Disconnected",(direction=='r')?"input":"output",devid);
  return 0;
}

static int cb_input(struct io *io,int devid,const void *src,int srcc) {
  return io_broadcast(io,src,srcc);
}

int main(int argc,char **argv) {
  fprintf(stderr,"%s: Starting up. SIGINT to quit.\n",argv[0]);
  signal(SIGINT,rcvsig);
  struct io_delegate delegate={
    .change=cb_change,
    .input=cb_input,
  };
  if (!(io=io_new(&delegate))) return 1;
  while (!sigc) {
    if (io_update(io,1000)<0) {
      fprintf(stderr,"Error updating.\n");
      io_broadcast(io,"\xff",1);
      io_del(io);
      return 1;
    }
  }
  io_broadcast(io,"\xff",1);
  io_del(io);
  fprintf(stderr,"%s: Normal exit.\n",argv[0]);
  return 0;
}
