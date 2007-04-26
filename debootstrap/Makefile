TCOS_BINS:=/usr/lib/tcos
DESTDIR:=/

all: compile
	#none

compile:
	gcc -Wall pkgdetails.c -o pkgdetails

clean:
	rm -f pkgdetails *~

install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -m 755 pkgdetails $(DESTDIR)/$(TCOS_BINS)/
