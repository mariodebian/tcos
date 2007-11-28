PROGRAMS = lockscreen
SOURCES = lockscreen2.c
LIBS=$(shell imlib-config --libs)
CFLAGS=$(shell imlib-config --cflags)
LDADD = -Wall -lXaw $(LIBS) $(CFLAGS)


all: compile

include ../common.mk


compile:
	$(CC) $(SOURCES) $(LDADD) -o $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) *~

debug: clean
	$(CC) $(SOURCES) $(LDADD) -o $(PROGRAMS) -DDEBUG=1

install:
	mkdir -p $(DESTDIR)/$(TCOS_BINS)
	mkdir -p $(DESTDIR)/$(PREFIX)/share/tcos-core/

	install -m 755 lockscreen $(DESTDIR)/$(TCOS_BINS)/lockscreen
	install -m 644 lockscreen.png $(DESTDIR)/$(PREFIX)/share/tcos-core/lockscreen.png
