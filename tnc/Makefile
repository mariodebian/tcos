TNC = tnc
SOURCE = tcos-net-controller.c
LIBS= -I/usr/lib
CFLAGS = -O2 -ansi -pedantic -Wall
GCC=gcc

all: compile

include ../common.mk


compile:
	$(CC) $(SOURCE) $(LDADD) -o $(TNC)

clean:
	rm -f $(TNC) *~

install:
	mkdir -p $(DESTDIR)/$(TCOS_BINS)

	install -m 700 $(TNC)        $(DESTDIR)/$(TCOS_BINS)/

