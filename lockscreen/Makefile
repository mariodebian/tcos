LOCK = lockscreen
XRES = screensize
KEYBMOUSE = lockkeybmouse
SOURCES = lockscreen2.c
XRES_SOURCES = screensize.c
KEYBMOUSE_SOURCES = lockkeybmouse.c
LIBS= -I/usr/lib
LOCKLIBS=$(shell imlib-config --libs)
CFLAGS=$(shell imlib-config --cflags)
LDADD = -Wall -lXaw $(LIBS) $(CFLAGS)


all: compile

include ../common.mk


compile:
	$(CC) $(SOURCES) $(LOCKLIBS) $(LDADD) -o $(LOCK)
	$(CC) $(XRES_SOURCES) $(LDADD) -o $(XRES)
	$(CC) $(KEYBMOUSE_SOURCES) $(LDADD) -o $(KEYBMOUSE)

clean:
	rm -f $(LOCK) $(XRES) $(KEYBMOUSE) *~

debug: clean
	$(CC) $(SOURCES) $(LOCKLIBS) $(LDADD) -o $(LOCK) -DDEBUG=1
	$(CC) $(KEYBMOUSE_SOURCES) $(LDADD) -o $(KEYBMOUSE) -DDEBUG=1

install:
	mkdir -p $(DESTDIR)/$(TCOS_BINS)
	mkdir -p $(DESTDIR)/$(PREFIX)/share/tcos-core/

	install -m 755 $(LOCK)        $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 $(XRES)        $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 $(KEYBMOUSE)   $(DESTDIR)/$(TCOS_BINS)/
	install -m 644 lockscreen.png $(DESTDIR)/$(PREFIX)/share/tcos-core/lockscreen.png
