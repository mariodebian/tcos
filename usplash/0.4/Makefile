CC=gcc
CFLAGS=-g -Wall -fPIC
LDFLAGS=
INCLUDES=

COMPILE = $(CC) $(INCLUDES) $(CFLAGS)
LINK = $(CC) $(CFLAGS) $(LDFLAGS)

INSTALL = /usr/bin/install -c
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_PROGRAM = $(INSTALL) -m 755

usplash-tcos.so: throbber_back.png.c.o throbber_back_16.png.c.o throbber_fore.png.c.o throbber_fore_16.png.c.o \
						 usplash_1024_768.png.c.o usplash_800_600.png.c.o \
						 usplash_640_400.png.c.o usplash_640_480.png.c.o \
						 usplash-tcos.c.o helvB10.bdf.c.o
	$(COMPILE) -shared -o $@ $^

%.png.c: %.png
	pngtousplash $< > $@

%.bdf.c: %.bdf
	bdftousplash $< > $@

%.c.o: %.c
	$(COMPILE) -o $@ -c $<

install:
	$(INSTALL) -d $(DESTDIR)/usr/lib/usplash
	$(INSTALL_PROGRAM) usplash-tcos.so $(DESTDIR)/usr/lib/usplash/usplash-tcos.so
clean:
	rm -f *.png.c *.bdf.c *.c.o *.so
