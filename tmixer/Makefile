CFLAGS		= -g -O2 -Wall -ansi -pedantic -std=c99 -DTCOS_VERSION="\"$(VERSION)\""
LDADD		= -lasound
bin_PROGRAMS = tmixer
TMIXER_SOURCES= tmixer-alsa.c tmixer-oss.c
CC=gcc

tmixer: tmixer.c tmixer-oss.c tmixer-oss.h tmixer-alsa.c tmixer-alsa.h
	$(CC) -o $(bin_PROGRAMS) tmixer.c $(LDADD) $(CFLAGS)

tmixer-alsa: tmixer-alsa.c tmixer-alsa.h
	$(CC) -o $(bin_PROGRAMS) tmixer-alsa.c $(LDADD) $(CFLAGS)

tmixer-oss: tmixer-oss.c tmixer-oss.h
	$(CC) -o $(bin_PROGRAMS) tmixer-oss.c $(LDADD) $(CFLAGS)


include ../common.mk

clean:
	rm -f *~ $(bin_PROGRAMS) *.o

install:
	mkdir -p $(DESTDIR)/$(TCOS_BINS)
	install -m 755 $(bin_PROGRAMS)        $(DESTDIR)/$(TCOS_BINS)/

