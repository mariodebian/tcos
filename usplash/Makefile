all:
	make $(DISTRO)-usplash

include ../common.mk

vars:
	@echo IMAGE=$(IMAGE)
	@echo SOFILE=$(SOFILE)
	@echo DISTRO=$(DISTRO)

clean:
	make $(DISTRO)-usplash-clean

install:
	make $(DISTRO)-usplash-install

debian-usplash:
	# nothing !!!
#	pngtobogl $(IMAGE) > $(IMAGE).c
#	gcc -Os -g -I/usr/include/bogl -fPIC -c $(IMAGE).c -o $(IMAGE).c.o
#	gcc -shared -Wl,-soname,usplash-artwork.so $(IMAGE).c.o -o $(SOFILE)
	

ubuntu-usplash:
	cd ubuntu-tcos && make

ubuntu-usplash-clean:
	cd ubuntu-tcos && make clean

debian-usplash-clean:
	# none
#	rm -f *png.c *.o $(SOFILE)

	

debian-usplash-install:
	install -d $(DESTDIR)/usr/lib/usplash/
	install -m 755 usplash-tcos.so $(DESTDIR)/usr/lib/usplash/usplash-tcos.so

ubuntu-usplash-install:
	cd ubuntu-tcos && make install DESTDIR=$(DESTDIR)




#	install -m 755 usplash-tcos.so /var/lib/dpkg/alternatives/usplash-artwork.so
#	install -m 755 usplash-tcos.so /usr/lib/usplash/usplash-artwork.so
#	install -m 755 usplash-tcos.so /usr/lib/usplash/usplash-default.so
