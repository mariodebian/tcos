all: hex2ascii

include ../common.mk

hex2ascii:
	$(QUIET_CC)$(GCC) hex2ascii.c -o hex2ascii -Wall -ansi -pedantic

clean:
	$(QUIET_CL)rm -f hex2ascii

install:
	install -d $(DESTDIR)$(TCOS_BINS)
	install -m 755 hex2ascii $(DESTDIR)$(TCOS_BINS)/hex2ascii
