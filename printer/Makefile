all: compile

include ../common.mk

compile:
	gcc -Wall -O2 -ansi -pedantic print_server.c -o print_server

clean:
	rm -f print_server *~

install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -m 755 print_server $(DESTDIR)/$(TCOS_BINS)/

exe:
	./print_server /home/mario/prueba.txt

debug: clean compile exe
