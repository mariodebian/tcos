all:
	make $(USPLASH_VERSION)-usplash

include ../common.mk


USPLASH_VERSION=$(shell bash test.sh)

vars:
	@echo DISTRO=$(DISTRO)
	@echo USPLASH_VERSION=$(USPLASH_VERSION)

clean:
	make $(USPLASH_VERSION)-usplash-clean

install:
	make $(USPLASH_VERSION)-usplash-install

0.2-usplash:
	cd 0.2 && make

0.3-usplash:
	cd 0.3 && make

0.4-usplash:
	cd 0.4 && make


0.2-usplash-clean:
	cd 0.2 && make clean

0.3-usplash-clean:
	cd 0.3 && make clean

0.4-usplash-clean: 
	cd 0.4 && make clean

0.2-usplash-install:
	cd 0.2 && make install DESTDIR=$(DESTDIR)

0.3-usplash-install:
	cd 0.3 && make install DESTDIR=$(DESTDIR)

0.4-usplash-install:
	cd 0.4 && make install DESTDIR=$(DESTDIR)

