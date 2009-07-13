SERVER=tcosxmlrpc
STANDALONE=tcosxmlrpc-standalone
TCOSPASSWD=tcospasswd

all: message $(SERVER) $(STANDALONE) $(TCOSPASSWD)

include ../common.mk



TCOS_PATH:=/sbin
IS_STANDALONE:=0

ifneq ($(findstring $(IS_STANDALONE),1),1)
TXT = 
else
TXT = STANDALONE
endif

ifndef V
	QUIET_CC       = @echo '   $(TXT)'  CC    $@;
	QUIET_CL       = @echo '   $(TXT)'  CLEAN $@ `basename $(CURDIR)`;
	QUIET_LN       = @echo '   $(TXT)'  LINK  $@;
endif



XMLRPC_INC=$(shell xmlrpc-c-config --version)
XMLRPC_VERSION_MAYOR=$(shell xmlrpc-c-config --version| awk -F"." '{print $$1}')
XMLRPC_VERSION_MINOR=$(shell xmlrpc-c-config --version| awk -F"." '{print $$2$$3}'| cut -c-4| awk '{print int($$1)}')

CFLAGS+=-Wall -O2  `xmlrpc-c-config abyss-server --cflags`
#LDFLAGS+=-Wl,-z,defs -Wl,-O1 -Wl,--as-needed
## OLD gcc (lenny) don't work with -Wl,--as-needed
LDFLAGS+=-Wl,-z,defs -Wl,-O1 
LIBS+=`xmlrpc-c-config abyss-server --libs` -lcrypt -lX11

DEFINES:= -DVERSION=\"$(VERSION)\" -DXMLRPC_VERSION_MAYOR=$(XMLRPC_VERSION_MAYOR) -DXMLRPC_VERSION_MINOR=$(XMLRPC_VERSION_MINOR)

#TEST_DEFINES=-DDEBUG -DVERSION=\"$(VERSION)\" -DBSIZE=512 -DVISIBLE_PASSWD -DXMLRPC_INC=$(XMLRPC_INC) -DXMLRPC_VERSION_MAYOR=$(XMLRPC_VERSION_MAYOR) -DXMLRPC_VERSION_MINOR=$(XMLRPC_VERSION_MINOR)



SRCS	     := common.c \
		debug.c \
		devices.c \
		dpms.c \
		exe.c \
		get_screenshot.c \
		info.c \
		lockcontroller.c \
		lockscreen.c \
		login.c \
		pam-usb.c \
		pci.c \
		reboot_poweroff.c \
		rtp.c \
		simple-methods.c \
		sound.c \
		standalone.c \
		tnc.c \
		validate.c \
		vlc.c \
		vnc.c \
		xauth.c \
		xorg.c \
		tcosxmlrpc.c

# deleted parts
#		screenshot.c \
#

OBJS		:= $(SRCS:.c=.o)

message:
	@echo " * Building tcosxmlrpc"

cleanobjs:
	$(QUIET_CL)rm -f $(OBJS)

%.o: %.c
	$(QUIET_CC)$(GCC) -c ${CFLAGS} -Wno-unused-function ${DEFINES} -DTCOS_PATH=\"$(TCOS_PATH)\" $< -o $@

$(SERVER): $(OBJS)
	$(QUIET_LN)$(GCC) $(LDFLAGS) $(DEFINES) -o $(SERVER) $(CFLAGS) $(OBJS) $(LIBS)
	$(MAKE) cleanobjs

$(STANDALONE):
	$(MAKE) $(OBJS) TCOS_PATH=$(TCOS_BINS) IS_STANDALONE=1 CFLAGS+='-DIS_STANDALONE=1'
	$(QUIET_LN)$(GCC) $(LDFLAGS) $(DEFINES) -o $(STANDALONE) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)
	$(MAKE) cleanobjs

$(TCOSPASSWD):
	$(QUIET_CC)$(GCC) $(DEFINES) $(CFLAGS) -o $(TCOSPASSWD) $(TCOSPASSWD).c -lcrypt



test2:
	@echo XMLRPC_INC=$(XMLRPC_INC)
	@echo XMLRPC_VERSION_MAYOR=$(XMLRPC_VERSION_MAYOR)
	@echo XMLRPC_VERSION_MINOR=$(XMLRPC_VERSION_MINOR)


clean:
	$(QUIET_CL)rm -f $(OBJS) $(SERVER) $(STANDALONE) $(TCOSPASSWD) var/log.txt var/log/* *~ image*.jpg image*png base64 sh/*~

install:
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -d $(DESTDIR)/$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/
	install -d $(DESTDIR)/$(PREFIX)/sbin
    
	install -m 755 $(SERVER)       $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 $(STANDALONE)   $(DESTDIR)/$(TCOS_BINS)/

	install -m 755 $(TCOSPASSWD) $(DESTDIR)/$(PREFIX)/sbin/$(TCOSPASSWD)
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
