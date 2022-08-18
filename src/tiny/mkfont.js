#!/usr/bin/env node
const fs = require("fs");

// If storage size is every a concern:
// We could store the font 1-bit, and eliminate some spacing (eg there is always 1 blank column at the right, and some blank rows somewhere).
// I don't think it's worth the trouble.

const SRCW = 96;
const SRCH = 48;

if (process.argv.length !== 4) {
  throw new Error(`Usage: ${process.argv[1]} INPUT OUTPUT\nINPUT is 8-bit grayscale, OUTPUT is C.\n`);
}

const [_a, _b, srcpath, dstpath] = process.argv;

const src = fs.readFileSync(srcpath);
if (src.length !== SRCW * SRCH) {
  throw new Error(`${srcpath}: Expected length ${SRCW}*${SRCH}=${SRCW*SRCH}, found ${src.length}`);
}

let dst = "#include <stdint.h>\n#include <avr/pgmspace.h>\n";
dst += "const uint8_t font[] PROGMEM={\n";
for (let srcp=0; srcp<src.length; ) {
  for (let i=0; i<24; i++, srcp++) {
    dst += `${src[srcp]},`;
  }
  dst += "\n";
}
dst+="};\n";

fs.writeFileSync(dstpath, dst);
