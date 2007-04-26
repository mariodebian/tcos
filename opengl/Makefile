###
###   Get OpenGL libs
###



all: testlibs


include ../common.mk


LIBGL := $(wildcard /usr/lib/libGL.so)
LIBGL := $(wildcard /usr/lib/libGL.so.1)
LIBGL := $(wildcard /usr/lib/libGL.so.1.2)

LIBGLU := $(wildcard /usr/lib/libGLU.so)
LIBGLU := $(wildcard /usr/lib/libGLU.so.1)


testvars:
	@echo LIBGL=$(LIBGL)
	@echo LIBGLU=$(LIBGL)

testlibs:
	@if [ "$$(strings $(LIBGL) |grep -c NVIDIA)" != 0 ]; then\
		echo "**************************************" ;\
		echo "Please remove NVIDIA drivers first!!!!" ;\
		echo "**************************************" ;\
		exit 1;\
	else \
		echo "**************************************" ;\
		echo "      NVIDIA libGL pass test OK !!!!  " ;\
		echo "**************************************" ;\
	fi

	@if [ "$$(strings $(LIBGL) |grep -c -i fgl)" != 0 ]; then\
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
	cp -ra $$(readlink -f $(LIBGL) ) $(DESTDIR)/usr/lib/tcos/opengl/
	cp -ra $$(readlink -f $(LIBGLU) ) $(DESTDIR)/usr/lib/tcos/opengl/
	cp -ra /usr/lib/xorg/modules/extensions/libGLcore.so $(DESTDIR)/usr/lib/tcos/opengl/
