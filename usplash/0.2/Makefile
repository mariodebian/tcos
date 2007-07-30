all: usplash

clean:
	rm -f *.o *.so *.lo usplash-tcos.so usplash-artwork.so usplash-artwork.c pngtobogl 


usplash:
	gcc -g -D_GNU_SOURCE -Wall -D_GNU_SOURCE -DBOGL_CFB_FB=1 -DBOGL_VGA16_FB=1 -o pngtobogl.o -c pngtobogl.c
	gcc -g -D_GNU_SOURCE -Wall -D_GNU_SOURCE -DBOGL_CFB_FB=1 -DBOGL_VGA16_FB=1 -o pngtobogl pngtobogl.o -lgd -lpng -lm
	./pngtobogl usplash-artwork.png > usplash-artwork.c

	gcc -g -D_GNU_SOURCE -Wall -D_GNU_SOURCE -DBOGL_CFB_FB=1 -DBOGL_VGA16_FB=1 -o usplash-artwork.lo -fPIC -c usplash-artwork.c
	gcc -shared -Wl,-soname,usplash-artwork.so usplash-artwork.lo -o usplash-artwork.so
	mv usplash-artwork.so usplash-tcos.so


install:
	install -d $(DESTDIR)/usr/lib/usplash
	install -m 755 usplash-tcos.so $(DESTDIR)/usr/lib/usplash/usplash-tcos.so
