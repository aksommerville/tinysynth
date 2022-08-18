ifeq ($(MAKECMDGOALS),clean)

.SILENT:
clean:;rm -rf mid out

else

all:
.SILENT:
.SECONDARY:
PRECMD=echo "  $(@F)" ; mkdir -p $(@D) ;

SRCFILES:=$(shell find src -type f)

include etc/make/native.mk
include etc/make/tiny.mk

run:tiny-run native-run

endif
