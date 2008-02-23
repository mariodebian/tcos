all: compile

include ../common.mk

compile:
	gcc -Wall -O2 -ansi -pedantic pkgdetails.c -o pkgdetails

clean:
	rm -f pkgdetails *~

install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -m 755 pkgdetails $(DESTDIR)/$(TCOS_BINS)/
