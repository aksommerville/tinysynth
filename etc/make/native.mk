# native.mk
# Rules for building everything that runs on the PC.

native_CC:=gcc -c -MMD -O3 -Isrc -Werror -Wimplicit -DTS_native=1
native_LD:=gcc
native_LDPOST:=-lm -lasound -lpthread

native_MIDDIR:=mid/native
native_SRCFILES:=$(filter src/native/% src/common/%,$(SRCFILES))

native_CFILES:=$(filter %.c,$(native_SRCFILES))
native_OFILES:=$(patsubst src/%.c,$(native_MIDDIR)/%.o,$(native_CFILES))
-include $(native_OFILES:.o=.d)

$(native_MIDDIR)/%.o:src/%.c;$(PRECMD) $(native_CC) -o $@ $<

native_EXE:=out/ts-host
all:$(native_EXE)

$(native_EXE):$(native_OFILES);$(PRECMD) $(native_LD) -o $@ $^ $(native_LDPOST)

native-run:$(native_EXE);$(native_EXE)
