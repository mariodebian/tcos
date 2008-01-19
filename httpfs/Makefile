all: httpfs

include ../common.mk

CFLAGS=-O2 -g -Wall

FUSE_CFLAGS=$(shell pkg-config fuse --cflags)
FUSE_LDD=$(shell pkg-config fuse --libs)
FUSE_VERSION=-DFUSE_USE_VERSION=$(shell pkg-config fuse --modversion| awk -F "." '{print $$1""$$2}')

HTTPFS_FLAGS=-DUSE_AUTH -D_REENTRANT -D_POSIX_C_SOURCE=200112L -D_POSIX_SOURCE -D_SVID_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=500

clean:
	rm -f *~ httpfs httpfs.o

httpfs: httpfs.c
	gcc -c $(CFLAGS) $(FUSE_CFLAGS) $(HTTPFS_FLAGS) $(FUSE_VERSION) httpfs.c
	gcc -s httpfs.o -o httpfs $(FUSE_LDD)


install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -m 755 httpfs $(DESTDIR)/$(TCOS_BINS)/
