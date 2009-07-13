LOCK = lockscreen
XRES = screensize
LOCKVNC = lockvnc
LOCKVLC = lockvlc
SOURCES = lockscreen2.c
XRES_SOURCES = screensize.c
LOCKVNC_SOURCES = lockvnc.c
LOCKVLC_SOURCES = lockvlc.c
LIBS= -I/usr/lib
LOCKLIBS=$(shell pkg-config --libs imlib2)
CFLAGS=$(shell pkg-config --cflags imlib2)
LDADD=-O2 -ansi -pedantic -Wall -lXaw $(LIBS) $(CFLAGS)
CC=gcc


all: $(LOCK) $(XRES) $(LOCKVNC) $(LOCKVLC)

include ../common.mk


$(LOCK):
	$(QUIET_CC)$(CC) $(SOURCES) $(LOCKLIBS) $(LDADD) -o $(LOCK)

$(XRES):
	$(QUIET_CC)$(CC) $(XRES_SOURCES) $(LDADD) -o $(XRES)

$(LOCKVNC):
	$(QUIET_CC)$(CC) $(LOCKVNC_SOURCES) $(LDADD) -o $(LOCKVNC)

$(LOCKVLC):
	$(QUIET_CC)$(CC) $(LOCKVLC_SOURCES) $(LDADD) -o $(LOCKVLC)

clean:
	$(QUIET_CL)rm -f $(LOCK) $(XRES) $(LOCKVNC) $(LOCKVLC) *~

debug: clean
	$(CC) $(SOURCES) $(LOCKLIBS) $(LDADD) -o $(LOCK) -DDEBUG=1
	$(CC) $(LOCKVNC_SOURCES) $(LDADD) -o $(LOCKVNC) -DDEBUG=1
	$(CC) $(LOCKVLC_SOURCES) $(LDADD) -o $(LOCKVLC) -DDEBUG=1

install:
	mkdir -p $(DESTDIR)/$(TCOS_BINS)
	mkdir -p $(DESTDIR)/$(PREFIX)/share/tcos-core/

	install -m 755 $(LOCK)        $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 $(XRES)        $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 $(LOCKVNC)   $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 $(LOCKVLC)   $(DESTDIR)/$(TCOS_BINS)/
	install -m 644 lockscreen.png $(DESTDIR)/$(PREFIX)/share/tcos-core/lockscreen.png
