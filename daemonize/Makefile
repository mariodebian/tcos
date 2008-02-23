all: compile


include ../common.mk

compile:
	gcc -Wall -O2 -ansi -pedantic daemon.c -o daemonize

clean:
	rm -f daemonize daemon *~

install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -m 755 daemonize $(DESTDIR)/$(TCOS_BINS)/
