#.SILENT:

PACKAGE=initramfs-tools-tcos

MAKE=make -B
INSTALL=install

VERSION=$(shell head -1 debian/changelog 2>/dev/null | awk  '{gsub(/\(/,"",$$2); gsub(/\)/, "" , $$2); print $$2}' )

have_changelog := $(wildcard debian/changelog)
ifeq ($(strip $(have_changelog)),)
VERSION=$(shell head -1 ../debian/changelog 2>/dev/null | awk  '{gsub(/\(/,"",$$2); gsub(/\)/, "" , $$2); print $$2}' )
endif


TCOS_CONFIG_FILE=conf/tcos.conf
have_config := $(wildcard conf/tcos.conf)
ifeq ($(strip $(have_config)),)
TCOS_CONFIG_FILE=../conf/tcos.conf
endif

TCOS_DIR=$(shell awk -F "=" '/TCOS_DIR=/ {print $$2}' $(TCOS_CONFIG_FILE) )
TFTP_DIR=$(shell awk -F "=" '/TFTP_DIR=/ {print $$2}' $(TCOS_CONFIG_FILE) )
TCOS_CONF=$(shell awk -F "=" '/TCOS_CONF=/ {print $$2}' $(TCOS_CONFIG_FILE) )
TCOS_BINS=$(shell awk -F "=" '/TCOS_BINS=/ {print $$2}' $(TCOS_CONFIG_FILE) )



TCOS_XMLRPC_DIR=$(PREFIX)/share/tcosmonitor/xmlrpc/
DBUS_CONF=/etc/dbus-1/system.d/
X11_CONF=/etc/X11/Xsession.d/



# debian or ubuntu ???
HAVE_DEBIAN=$(shell grep -i -c debian /etc/issue)
HAVE_UBUNTU=$(shell grep -i -c ubuntu /etc/issue)

ifeq ($(HAVE_DEBIAN),1)
DISTRO=debian
endif

ifeq ($(HAVE_UBUNTU),1)
DISTRO=ubuntu
endif


ifeq ($(strip $(DISTRO)),) 
DISTRO=debian
endif

ifeq ($(DISTRO),debian) 
DEB_MIRROR=http://ftp.uk.debian.org/debian/
endif

ifeq ($(DISTRO),ubuntu)
DEB_MIRROR=http://archive.ubuntu.com/ubuntu/
endif






PREFIX:=/usr


test:
	@echo "------------------------------------"
	@echo VERSION=$(VERSION)
	@echo PACKAGE=$(PACKAGE)
	@echo 
	@echo PREFIX=$(PREFIX)
	@echo DESTDIR=$(DESTDIR)
	@echo
	@echo CURDIR=$(CURDIR)
	@echo 
	@echo TCOS_DIR=$(TCOS_DIR)
	@echo TCOS_BINS=$(TCOS_BINS)
	@echo TCOS_XMLRPC_DIR=$(TCOS_XMLRPC_DIR)
	@echo DBUS_CONF=$(DBUS_CONF)
	@echo x11_CONF=$(X11_CONF)
	@echo
	@echo DISTRO=$(DISTRO)
	@echo DEB_MIRROR=$(DEB_MIRROR)
	@echo "------------------------------------"