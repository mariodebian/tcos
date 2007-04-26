all: 
	if [ "$(shell bash test.sh)" = "1" ]; then \
		make usplash_dapper; \
	else \
		make usplash; \
	fi


clean:
	rm -f *.o *.so helvB10.c usplash-tcos.c


usplash:
	pngtobogl ./usplash-tcos.png > usplash-tcos.c
	bdftobogl helvB10.bdf > helvB10.c

	gcc -Os -g -I/usr/include/bogl -I. -fPIC -c usplash-tcos.c -o usplash-tcos.o
	gcc -Os -g -I/usr/include/bogl -I. -fPIC -c tcos-usplash-theme.c -o tcos-usplash-theme.o
	gcc -Os -g -I/usr/include/bogl -I. -fPIC -c helvB10.c -o helvB10.o
	gcc -shared -Wl,-soname,usplash-tcos.so usplash-tcos.o tcos-usplash-theme.o helvB10.o -o usplash-tcos.so

usplash_dapper:
	pngtobogl ./usplash-tcos-600x400.png > usplash-tcos.c
	bdftobogl helvB10.bdf > helvB10.c
	gcc -Os -g -I/usr/include/bogl -I. -fPIC -c tcos-usplash-theme.c -o tcos-usplash-theme.o
	gcc -Os -g -I/usr/include/bogl -I. -fPIC -c helvB10.c -o helvB10.o

	gcc -g -D_GNU_SOURCE -Wall -I/usr/include/bogl -D_GNU_SOURCE -DBOGL_CFB_FB=1 -DBOGL_VGA16_FB=1 -o usplash-tcos.o -fPIC -c usplash-tcos.c
	gcc -shared -Wl,-soname,usplash-tcos.so usplash-tcos.o tcos-usplash-theme.o helvB10.o -o usplash-tcos.so


install:
	install -d $(DESTDIR)/usr/lib/usplash
	install -m 755 usplash-tcos.so $(DESTDIR)/usr/lib/usplash/usplash-tcos.so
