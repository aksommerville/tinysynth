#include "io.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/poll.h>
#include <sys/inotify.h>

#define IO_DIR "/dev"

/* Instance definition.
 */
 
struct io {
  struct io_delegate delegate;
  int infd;
  struct io_file {
    int fd;
    char direction;
    int devid;
  } *filev;
  int filec,filea;
  int scan;
  struct pollfd *pollfdv;
  int pollfda;
};

/* Cleanup.
 */
 
static void io_file_cleanup(struct io_file *file) {
  if (file->fd>=0) close(file->fd);
}
 
void io_del(struct io *io) {
  if (!io) return;
  if (io->infd>=0) close(io->infd);
  if (io->pollfdv) free(io->pollfdv);
  if (io->filev) {
    while (io->filec-->0) io_file_cleanup(io->filev+io->filec);
    free(io->filev);
  }
  free(io);
}

/* New.
 */

struct io *io_new(const struct io_delegate *delegate) {
  if (!delegate) return 0;
  struct io *io=calloc(1,sizeof(struct io));
  if (!io) return 0;
  
  io->delegate=*delegate;
  io->scan=1;
  
  if ((io->infd=inotify_init())>=0) {
    inotify_add_watch(io->infd,IO_DIR,IN_CREATE|IN_ATTRIB|IN_MOVED_TO);
  }
  
  return io;
}

/* Devid and direction from basename.
 */
 
static int io_parse_basename(char *direction,struct io *io,const char *base,int basec) {
  
  if (io->delegate.use_oss_midi&&(basec>=4)&&!memcmp(base,"midi",4)) {
    *direction='r';
    base+=4;
    basec-=4;
  } else if (io->delegate.use_tiny&&(basec>=6)&&!memcmp(base,"ttyACM",6)) {
    *direction='w';
    base+=6;
    basec-=6;
  } else return -1;
  
  if ((basec<1)||(basec>9)) return -1; // 9 digit limit prevents overflow
  int devid=0;
  for (;basec-->0;base++) {
    if ((*base<'0')||(*base>'9')) return -1;
    devid*=10;
    devid+=(*base)-'0';
  }
  return devid;
}

/* Find file.
 */
 
static struct io_file *io_find_file_by_devid(const struct io *io,char direction,int devid) {
  struct io_file *file=io->filev;
  int i=io->filec;
  for (;i-->0;file++) {
    if (file->direction!=direction) continue;
    if (file->devid!=devid) continue;
    return file;
  }
  return 0;
}

static struct io_file *io_find_file_by_fd(const struct io *io,int fd) {
  struct io_file *file=io->filev;
  int i=io->filec;
  for (;i-->0;file++) {
    if (file->fd!=fd) continue;
    return file;
  }
  return 0;
}

/* Add file.
 * Caller must check whether it exists.
 * Failure to open is not an error.
 * We trigger the callback.
 */
 
static int io_add_file(struct io *io,const char *path,char direction,int devid) {

  if (io->filec>=io->filea) {
    int na=io->filea+8;
    if (na>INT_MAX/sizeof(struct io_file)) return -1;
    void *nv=realloc(io->filev,sizeof(struct io_file)*na);
    if (!nv) return -1;
    io->filev=nv;
    io->filea=na;
  }
  
  int fd=open(path,(direction=='r')?O_RDONLY:O_WRONLY);
  if (fd<0) return 0;
  
  struct io_file *file=io->filev+io->filec++;
  memset(file,0,sizeof(struct io_file));
  file->fd=fd;
  file->direction=direction;
  file->devid=devid;
  
  if (io->delegate.change) {
    if (io->delegate.change(io,devid,direction,1)<0) return -1;
  }

  return 1;
}

/* Scan directory.
 */
 
static int io_scan(struct io *io) {
  DIR *dir=opendir(IO_DIR);
  if (!dir) return -1;
  struct dirent *de;
  while (de=readdir(dir)) {
    char direction;
    int devid;
    int basec=0;
    while (de->d_name[basec]) basec++;
    if ((devid=io_parse_basename(&direction,io,de->d_name,basec))<0) continue;
    if (io_find_file_by_devid(io,direction,devid)) continue;
    char path[1024];
    if (snprintf(path,sizeof(path),"%s/%.*s",IO_DIR,basec,de->d_name)>=sizeof(path)) continue;
    if (io_add_file(io,path,direction,devid)<0) {
      closedir(dir);
      return -1;
    }
  }
  closedir(dir);
  return 0;
}

/* Read from inotify.
 * We use a fresh buffer each time, so there is a risk that events could get sliced.
 * I don't know if Linux actually does that. Not going to worry about it.
 */
 
static int io_update_inotify(struct io *io) {
  char buf[1024];
  int bufc=read(io->infd,buf,sizeof(buf));
  if (bufc<=0) {
    fprintf(stderr,"Lost inotify connection to '%s'. We will not detect further connections.\n",IO_DIR);
    close(io->infd);
    io->infd=-1;
    return 0;
  }
  int bufp=0;
  while (bufp+sizeof(struct inotify_event)<bufc) {
    struct inotify_event *event=(struct inotify_event*)(buf+bufp);
    bufp+=sizeof(struct inotify_event);
    bufp+=event->len;
    const char *base=event->name;
    int basec=0;
    while ((basec<event->len)&&base[basec]) basec++;
    char direction;
    int devid=io_parse_basename(&direction,io,base,basec);
    if (devid<0) continue;
    if (io_find_file_by_devid(io,direction,devid)) continue;
    char path[1024];
    if (snprintf(path,sizeof(path),"%s/%.*s",IO_DIR,basec,base)>=sizeof(path)) continue;
    if (io_add_file(io,path,direction,devid)<0) return -1;
  }
  return 0;
}

/* Drop file by fd.
 * Triggers callback.
 */
 
static int io_drop_file(struct io *io,int fd) {
  struct io_file *file=io->filev;
  int i=0;
  for (;i<io->filec;i++,file++) {
    if (file->fd!=fd) continue;
    struct io_file tmp=*file;
    io->filec--;
    memmove(file,file+1,sizeof(struct io_file)*(io->filec-i));
    io_file_cleanup(&tmp);
    if (io->delegate.change) {
      return io->delegate.change(io,tmp.devid,tmp.direction,0);
    }
    return 0;
  }
  return 0;
}

/* Read from file and react.
 */
 
static int io_update_fd(struct io *io,int fd) {
  if (fd==io->infd) return io_update_inotify(io);
  struct io_file *file=io_find_file_by_fd(io,fd);
  if (!file) return -1;
  // If an output file polled, it is HUP or ERR, drop it.
  if (file->direction=='w') return io_drop_file(io,fd);
  char buf[256];
  int bufc=read(file->fd,buf,sizeof(buf));
  if (bufc<=0) return io_drop_file(io,fd);
  if (io->delegate.input) {
    return io->delegate.input(io,file->devid,buf,bufc);
  }
  return 0;
}

/* Rebuild pollfd list.
 */
 
static int io_rebuild_pollfdv(struct io *io) {
  
  // Easy upper limit, just go with it.
  if (1+io->filec>io->pollfda) {
    int na=(1+io->filec+8)&~7;
    void *nv=realloc(io->pollfdv,sizeof(struct pollfd)*na);
    if (!nv) return -1;
    io->pollfdv=nv;
    io->pollfda=na;
  }
  int c=0;
  
  if (io->infd>=0) {
    struct pollfd *pollfd=io->pollfdv+c++;
    pollfd->fd=io->infd;
    pollfd->events=POLLIN|POLLHUP|POLLERR;
    pollfd->revents=0;
  }
  
  const struct io_file *file=io->filev;
  int i=io->filec;
  for (;i-->0;file++) {
    //if (file->direction!='r') continue;
    struct pollfd *pollfd=io->pollfdv+c++;
    pollfd->fd=file->fd;
    if (file->direction=='r') pollfd->events=POLLIN|POLLHUP|POLLERR;
    else pollfd->events=POLLHUP|POLLERR;
    pollfd->revents=0;
  }
  
  return c;
}

/* Update.
 */

int io_update(struct io *io,int to_ms) {
  if (!io) return -1;
  
  if (io->scan) {
    io->scan=0;
    io_scan(io);
  }
  
  int pollfdc=io_rebuild_pollfdv(io);
  if (pollfdc<0) return -1;
  if (!pollfdc) {
    if (to_ms>0) usleep(to_ms*1000);
    return 0;
  }
  
  if (poll(io->pollfdv,pollfdc,to_ms)<=0) return 0;
  const struct pollfd *pollfd=io->pollfdv;
  for (;pollfdc-->0;pollfd++) {
    if (pollfd->revents) {
      if (io_update_fd(io,pollfd->fd)<0) return -1;
    }
  }
  
  return 1;
}

/* Broadcast.
 */
 
int io_broadcast(struct io *io,const void *src,int srcc) {
  if (!io||(srcc<0)||(srcc&&!src)) return -1;
  int i=io->filec;
  struct io_file *file=io->filev+i-1;
  for (;i-->0;file--) {
    if (file->direction!='w') continue;
    if (write(file->fd,src,srcc)!=srcc) {
      struct io_file tmp=*file;
      io->filec--;
      memmove(file,file+1,sizeof(struct io_file)*(io->filec-i));
      io_file_cleanup(&tmp);
      if (io->delegate.change) {
        if (io->delegate.change(io,tmp.devid,tmp.direction,0)<0) return -1;
      }
    }
  }
  return 0;
}

/* Send.
 */
 
int io_send(struct io *io,int devid,const void *src,int srcc) {
  if (!io||(srcc<0)||(srcc&&!src)) return -1;
  struct io_file *file=io_find_file_by_devid(io,'w',devid);
  if (!file) return -1;
  if (write(file->fd,src,srcc)==srcc) return 0;
  return io_drop_file(io,file->fd);
}

/* Trivial accessors.
 */

void *io_get_userdata(const struct io *io) {
  if (!io) return 0;
  return io->delegate.userdata;
}
