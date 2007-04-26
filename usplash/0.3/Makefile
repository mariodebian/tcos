all: usplash

clean:
	rm -f *.o *.so helvB10.c usplash-tcos.c

usplash:
	pngtobogl ./usplash-tcos.png > usplash-tcos.c
	bdftobogl helvB10.bdf > helvB10.c

	gcc -Os -g -I/usr/include/bogl -I. -fPIC -c usplash-tcos.c -o usplash-tcos.o
	gcc -Os -g -I/usr/include/bogl -I. -fPIC -c tcos-usplash-theme.c -o tcos-usplash-theme.o
	gcc -Os -g -I/usr/include/bogl -I. -fPIC -c helvB10.c -o helvB10.o
	gcc -shared -Wl,-soname,usplash-tcos.so usplash-tcos.o tcos-usplash-theme.o helvB10.o -o usplash-tcos.so

install:
	install -d $(DESTDIR)/usr/lib/usplash
	install -m 755 usplash-tcos.so $(DESTDIR)/usr/lib/usplash/usplash-tcos.so
