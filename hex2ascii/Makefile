GCC=gcc
all: compile

include ../common.mk

compile:
	$(GCC) hex2ascii.c -o hex2ascii -Wall -ansi -pedantic

clean:
	rm -f hex2ascii

install:
	install -d $(DESTDIR)$(TCOS_BINS)
	install -m 755 hex2ascii $(DESTDIR)$(TCOS_BINS)/hex2ascii
