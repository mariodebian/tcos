TNC = tnc
SOURCE = tcos-net-controller.c
LIBS= -I/usr/lib
LDADD = -O2 -ansi -pedantic -Wall $(LIBS)
GCC=gcc

all: compile

include ../common.mk


compile:
	$(CC) $(SOURCE) $(LDADD) -o $(TNC)

debug:
	$(CC) $(SOURCE) $(LDADD) -o $(TNC) -DDEVNULL="\"\"" -DDEBUG


clean:
	rm -f $(TNC) *~

install:
	mkdir -p $(DESTDIR)/$(TCOS_BINS)

	install -m 700 $(TNC)        $(DESTDIR)/$(TCOS_BINS)/

