# tinysynth

Run a synthesizer on the TinyArcade, and pump MIDI events to it from the PC.

## TODO

- [x] Virtual MIDI receiver (via ALSA).
- xxx Send MIDI file (without input I/O). i guess not important
- [ ] Read synth config from files.
- [x] Realish synthesizer.
- [x] Play songs within synth.
- [ ] Foley. What are we doing for non-music sound?
- [x] ALSA output.
- [ ] snd_pcm_writei stalls when qtractor is open. what's the deal?

## Performance check

With a 512-sample buffer.
basicsquare: Underflow with 3 notes if I ride the pitch wheel.
fm: Underflow with 3 notes held.
fm without env: Underflow at 5 notes

Would it help to increase the buffer size? YES. 1024 is much better:
fm without env: Can't underflow
fm with env: Can't underflow (tho i do hear some crunch, what is that? ...probably clipping)

I'm not sure we'll be able to spare 2048 bytes in a real game.
Try the new buffer regime with Fullmoon.
...works great (just the driver bit, didn't try the new synth)
