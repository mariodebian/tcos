all: compile
	#none

include ../common.mk

compile:
	gcc -Wall daemonize.c -o daemonize

clean:
	rm -f daemonize *~

install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -m 755 daemonize $(DESTDIR)/$(TCOS_BINS)/
