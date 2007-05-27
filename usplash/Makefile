all:
	@echo "    => Compiling usplash $(USPLASH_VERSION)"
	cd $(USPLASH_VERSION) && make

include ../common.mk


USPLASH_VERSION=$(TCOS_USPLASH_VERSION)

vars:
	@echo DISTRO=$(DISTRO)
	@echo USPLASH_VERSION=$(USPLASH_VERSION)

clean:
	@echo "    => Cleaning usplash $(USPLASH_VERSION)"
	@cd $(USPLASH_VERSION) && make clean

install:
	@if [ "$(USPLASH_VERSION)" != "0.4.1" ]; then\
		echo "    => Installing usplash theme $(USPLASH_VERSION)";\
		( cd $(USPLASH_VERSION) && make install DESTDIR=$(DESTDIR) ); \
	fi

	@if [ "$(USPLASH_VERSION)" = "0.4.1" ]; then\
		echo "    => Installing usplash theme 0.4.1";\
		( cd $(USPLASH_VERSION) && make install DESTDIR=$(DESTDIR)/usr/lib/usplash/ );\
	fi
