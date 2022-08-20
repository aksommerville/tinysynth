/* io.h
 * Wraps inotify, poll, and file management.
 * We automatically open any /dev/midi* as input, and /dev/ttyACM* as output.
 */

#ifndef IO_H
#define IO_H

struct io;

struct io_delegate {
  void *userdata;
  int use_oss_midi;
  int use_tiny;
  
  /* Notify that a file opened or closed.
   * (devid) is derived from its path, eg "/dev/midi4" and "/dev/ttyACM4" are both devid 4.
   * (direction) is 'r' or 'w'.
   * (state) is 1=opened or 0=closed.
   * Note that (devid) is only unique for a given (direction).
   */
  int (*change)(struct io *io,int devid,char direction,int state);
  
  /* Data received from a MIDI device.
   * We don't broadcast automatically, but you'll probably want to.
   */
  int (*input)(struct io *io,int devid,const void *src,int srcc);
};

void io_del(struct io *io);

struct io *io_new(const struct io_delegate *delegate);

/* Call as often as possible.
 * Sleeps up to (to_ms) ms, 0 for no sleep, <0 for indefinite.
 * Returns >0 if something happened, 0 if timed out, or <0 for unusual errors.
 * Failing to open or read a file is not an error here.
 */
int io_update(struct io *io,int to_ms);

/* Send content to one or all outputs.
 * Typically, your (input) callback just calls io_broadcast() with the same payload.
 */
int io_broadcast(struct io *io,const void *src,int srcc);
int io_send(struct io *io,int devid,const void *src,int srcc);

void *io_get_userdata(const struct io *io);

#endif
