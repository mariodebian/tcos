all: compile


include ../common.mk

compile:
	gcc -Wall daemon.c -o daemonize

clean:
	rm -f daemonize daemon *~

install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -m 755 daemonize $(DESTDIR)/$(TCOS_BINS)/
