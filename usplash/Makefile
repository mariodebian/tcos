all:
	make $(USPLASH_VERSION)-usplash

include ../common.mk

USPLASH_VERSION=$$(if [ $$(ldd /sbin/usplash |grep -c libusplash) = 1 ]; then echo "0.4"; else echo "0.3"; fi)

vars:
	@echo DISTRO=$(DISTRO)
	@echo USPLASH_VERSION=$(USPLASH_VERSION)

clean:
	make $(USPLASH_VERSION)-usplash-clean

install:
	make $(USPLASH_VERSION)-usplash-install

0.3-usplash:
	cd 0.3 && make

0.4-usplash:
	cd 0.4 && make

0.3-usplash-clean:
	cd 0.3 && make clean

0.4-usplash-clean: 
	cd 0.4 && make clean

0.3-usplash-install:
	cd 0.3 && make install DESTDIR=$(DESTDIR)

0.4-usplash-install:
	cd 0.4 && make install DESTDIR=$(DESTDIR)

#debian-usplash:
#	# nothing !!!
#	pngtobogl $(IMAGE) > $(IMAGE).c
#	gcc -Os -g -I/usr/include/bogl -fPIC -c $(IMAGE).c -o $(IMAGE).c.o
#	gcc -shared -Wl,-soname,usplash-artwork.so $(IMAGE).c.o -o $(SOFILE)
#	
#
#ubuntu-usplash:
#	cd ubuntu-tcos && make

#ubuntu-usplash-clean:
#	cd ubuntu-tcos && make clean

#debian-usplash-clean:
#	# none
#	rm -f *png.c *.o $(SOFILE)

	

#debian-usplash-install:
#	install -d $(DESTDIR)/usr/lib/usplash/
#	install -m 755 usplash-tcos.so $(DESTDIR)/usr/lib/usplash/usplash-tcos.so
#
#ubuntu-usplash-install:
#	cd ubuntu-tcos && make install DESTDIR=$(DESTDIR)




#	install -m 755 usplash-tcos.so /var/lib/dpkg/alternatives/usplash-artwork.so
#	install -m 755 usplash-tcos.so /usr/lib/usplash/usplash-artwork.so
#	install -m 755 usplash-tcos.so /usr/lib/usplash/usplash-default.so
