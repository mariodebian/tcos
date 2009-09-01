TNC = tnc
SOURCE = tcos-net-controller.c
LIBS= -I/usr/lib
LDADD = -O2 -ansi -pedantic -Wall $(LIBS) $(CFLAGS)
GCC=gcc

all: $(TNC)

include ../common.mk


$(TNC):
	$(QUIET_CC)$(CC) $(SOURCE) $(LDADD) -o $(TNC)

debug:
	$(CC) $(SOURCE) $(LDADD) -o $(TNC) -DDEVNULL="\"\"" -DDEBUG


clean:
	$(QUIET_CL)rm -f $(TNC) *~

install:
	mkdir -p $(DESTDIR)/$(TCOS_BINS)

	install -m 700 $(TNC)        $(DESTDIR)/$(TCOS_BINS)/

