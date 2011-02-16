DEBUGCFLAGS	+= -g -O2 -Wall
CFLAGS		+= -g -O2 -Wall
TVERSION	= -DTCOS_VERSION="\"$(VERSION)\""
LDADD		+= -lasound -lm
bin_PROGRAMS = tmixer
TMIXER_SOURCES= tmixer-alsa.c tmixer-oss.c
CC=gcc

all: tmixer

tmixer: tmixer.c tmixer-oss.c tmixer-oss.h tmixer-alsa.c tmixer-alsa.h
	$(QUIET_CC)$(GCC) -o $(bin_PROGRAMS) tmixer.c $(LDADD) $(CFLAGS) $(TVERSION)

tmixer-alsa: tmixer-alsa.c tmixer-alsa.h
	$(QUIET_CC)$(GCC) -o $(bin_PROGRAMS) tmixer-alsa.c $(LDADD) $(CFLAGS) $(TVERSION)

tmixer-oss: tmixer-oss.c tmixer-oss.h
	$(QUIET_CC)$(GCC) -o $(bin_PROGRAMS) tmixer-oss.c $(LDADD) $(CFLAGS) $(TVERSION)


debug: tmixer.c tmixer-oss.c tmixer-oss.h tmixer-alsa.c tmixer-alsa.h
	$(CC) -o $(bin_PROGRAMS) tmixer.c $(LDADD) $(DEBUGCFLAGS) $(TVERSION)



include ../common.mk

clean:
	$(QUIET_CL)rm -f *~ $(bin_PROGRAMS) *.o

install:
	mkdir -p $(DESTDIR)/$(TCOS_BINS)
	install -m 755 $(bin_PROGRAMS)        $(DESTDIR)/$(TCOS_BINS)/

