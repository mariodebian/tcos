###
###   Get OpenGL libs
###



all: testlibs


include ../common.mk


testlibs:
	@if [ "$$(strings /usr/lib/libGL.so |grep -c NVIDIA)" != 0 ]; then\
		echo "**************************************" ;\
		echo "Please remove NVIDIA drivers first!!!!" ;\
		echo "**************************************" ;\
		exit 1;\
	else \
		echo "**************************************" ;\
		echo "      NVIDIA libGL pass test OK !!!!  " ;\
		echo "**************************************" ;\
	fi

	@if [ "$$(strings /usr/lib/libGL.so.1.2 |grep -c -i fgl)" != 0 ]; then\
		echo "**************************************" ;\
		echo " Please remove FGLRX drivers first!!!!" ;\
		echo "**************************************" ;\
		exit 1;\
	else \
		echo "**************************************" ;\
		echo "   FGLRX (ati) libGL pass test OK !!!!";\
		echo "**************************************" ;\
	fi

clean:
	rm -rf $(DESTDIR)/usr/lib/tcos/opengl

install: clean
	install -d $(DESTDIR)/usr/lib/tcos/opengl
	cp -ra $$(readlink -f /usr/lib/libGL.so) $(DESTDIR)/usr/lib/tcos/opengl/
	cp -ra $$(readlink -f /usr/lib/libGLU.so) $(DESTDIR)/usr/lib/tcos/opengl/
	cp -ra /usr/lib/xorg/modules/extensions/libGLcore.so $(DESTDIR)/usr/lib/tcos/opengl/
