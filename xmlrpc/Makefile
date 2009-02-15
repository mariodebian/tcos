all: server standalone tcospasswd


include ../common.mk


XMLRPC_INC=$(shell xmlrpc-c-config --version)
XMLRPC_VERSION_MAYOR=$(shell xmlrpc-c-config --version| awk -F"." '{print $$1}')
XMLRPC_VERSION_MINOR=$(shell xmlrpc-c-config --version| awk -F"." '{print $$2$$3}'| cut -c-4| awk '{print int($$1)}')
#CFLAGS=-Wall -std=c99 -O2 -ansi -pedantic `xmlrpc-c-config abyss-server --cflags`
CFLAGS=-Wall -O2  `xmlrpc-c-config abyss-server --cflags`
LIBS=`xmlrpc-c-config abyss-server --libs` -lcrypt -lX11
DEFINES= -DBSIZE=512 -DVERSION=\"$(VERSION)\" -DXMLRPC_VERSION_MAYOR=$(XMLRPC_VERSION_MAYOR) -DXMLRPC_VERSION_MINOR=$(XMLRPC_VERSION_MINOR)
TEST_DEFINES=-DDEBUG -DVERSION=\"$(VERSION)\" -DBSIZE=512 -DVISIBLE_PASSWD -DXMLRPC_INC=$(XMLRPC_INC) -DXMLRPC_VERSION_MAYOR=$(XMLRPC_VERSION_MAYOR) -DXMLRPC_VERSION_MINOR=$(XMLRPC_VERSION_MINOR)
GCC=gcc
SERVER=tcosxmlrpc

STANDALONE=tcosxmlrpc-standalone

test2:
	@echo XMLRPC_INC=$(XMLRPC_INC)
	@echo XMLRPC_VERSION_MAYOR=$(XMLRPC_VERSION_MAYOR)
	@echo XMLRPC_VERSION_MINOR=$(XMLRPC_VERSION_MINOR)

server:
	$(GCC) $(DEFINES) -o $(SERVER) $(CFLAGS) $(LIBS) $(SERVER).c -DTCOS_PATH=\"/sbin\"

standalone:
	$(GCC) $(DEFINES) -o $(STANDALONE) $(CFLAGS) $(LIBS) $(SERVER).c -DTCOS_PATH=\"$(TCOS_BINS)\" -DIS_STANDALONE=1

standalone-debug:
	$(GCC) $(TEST_DEFINES) -o $(STANDALONE) $(CFLAGS) $(LIBS) $(SERVER).c -DTCOS_PATH=\"$(TCOS_BINS)\" -DIS_STANDALONE=1 -DVISIBLE_PASSWD

debug:
	$(GCC) $(TEST_DEFINES) -o $(SERVER) $(CFLAGS) $(LIBS) $(SERVER).c -DTCOS_PATH=\"/sbin\"


tcospasswd:
	$(GCC) $(DEFINES) -o tcospasswd tcospasswd.c -lcrypt -O2

clean:
	rm -f $(SERVER) $(STANDALONE) tcosmd5 prueba prueba2 prueba3 validate-tcos validate-passwd tcospasswd var/log.txt var/log/* *~ image*.jpg image*png base64
	rm -f sh/*~

install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -d $(DESTDIR)/$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/
	install -d $(DESTDIR)/$(PREFIX)/sbin
    
	install -m 755 $(SERVER)       $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 $(STANDALONE)   $(DESTDIR)/$(TCOS_BINS)/

	install -m 755 tcospasswd $(DESTDIR)/$(PREFIX)/sbin/tcospasswd
	install -m 755 sh/update-tcospasswd.sh $(DESTDIR)/$(PREFIX)/sbin/update-tcospasswd
	
	# Install tcosxmlrpc utils
	install -m 755 sh/screenshot.sh    $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/soundctl.sh      $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/getinfo.sh       $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/useallmodules.sh $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/devicesctl.sh    $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/tcos-dbus.py     $(DESTDIR)/$(TCOS_BINS)/tcos-dbus-helper
	install -m 755 sh/vnc-controller.sh  $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/vlc-controller.sh  $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/rtp-controller.sh  $(DESTDIR)/$(TCOS_BINS)/
	#install -m 755 sh/clone-controller.sh  $(DESTDIR)/$(TCOS_BINS)/
	#install -m 755 sh/clonerecv          $(DESTDIR)/$(TCOS_BINS)/
	#install -m 755 sh/clonesend          $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/tcos-last          $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/pactl-controller.sh      $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 sh/tcos-pam-usb.sh    $(DESTDIR)/$(TCOS_BINS)/

	install -m 644 var/etc/mime.types  $(DESTDIR)/$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/


	install -m 644 var/etc/abyss.conf  $(DESTDIR)/$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/
	sed -i 's|__TCOS_ROOT__|/|g' $(DESTDIR)/$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/abyss.conf
	sed -i 's|__TCOS_WWW__|/var/www|g' $(DESTDIR)/$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/abyss.conf
	sed -i 's|__TCOS_LOG__|/var/log|g' $(DESTDIR)/$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/abyss.conf
	sed -i 's|__TCOS_ETC__|/etc|g' $(DESTDIR)/$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/abyss.conf

	# standalone package
	install -d $(DESTDIR)/$(TCOS_STANDALONE_DIR)
	install -d $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc
	install -d $(DESTDIR)/$(TCOS_STANDALONE_DIR)/www
	install -d $(DESTDIR)/$(TCOS_STANDALONE_DIR)/log


	install -m 644 var/etc/abyss.conf       $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/abyss-standalone.conf
	sed -i 's|__TCOS_ROOT__|$(TCOS_STANDALONE_DIR)|g' $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/abyss-standalone.conf
	sed -i 's|__TCOS_WWW__|$(TCOS_STANDALONE_DIR)/www|g' $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/abyss-standalone.conf
	sed -i 's|__TCOS_LOG__|$(TCOS_STANDALONE_DIR)/log|g' $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/abyss-standalone.conf
	sed -i 's|__TCOS_ETC__|$(TCOS_STANDALONE_DIR)/etc|g' $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/abyss-standalone.conf

	install -m 644 var/etc/mime.types       $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/



	# stunnel4
	install -d $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/stunnel
	install -d $(DESTDIR)/$(TCOS_STANDALONE_DIR)/lib/stunnel4/
	install -m 644 var/etc/stunnel/stunnel.conf         $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/stunnel
	install -m 644 var/etc/stunnel/stunnel-custom.conf  $(DESTDIR)/$(TCOS_STANDALONE_DIR)/etc/stunnel
