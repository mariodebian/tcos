#.SILENT:

PACKAGE=initramfs-tools-tcos

##################################
# kernel default versions

KERNEL_dapper="2.6.15-29-386"
USPLASH_dapper=0.2

KERNEL_edgy="2.6.17-12-generic"
USPLASH_edgy=0.4.1

KERNEL_feisty="2.6.20-16-generic"
USPLASH_feisty=0.4.1

KERNEL_etch="2.6.18-5-486"
USPLASH_etch=0.3

KERNEL_unstable="2.6.22-1-486"
USPLASH_unstable=0.4.1

KERNEL_experimental="2.6.22-1-486"
USPLASH_experimental=0.4.1

KERNEL_gutsy="2.6.22-10-generic"
USPLASH_gutsy=0.4.1
##################################


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
DISABLE_USPLASH=0

DISTRO_VERSION=$(shell dpkg-parsechangelog | awk '/^Distribution/ {print $$2}')
have_changelog := $(wildcard debian/changelog)
ifeq ($(strip $(have_changelog)),)
DISTRO_VERSION=$(shell dpkg-parsechangelog -l../debian/changelog | awk '/^Distribution/ {print $$2}')
endif

TCOS_DEFAULT_KERNEL=$(KERNEL_$(DISTRO_VERSION))
TCOS_USPLASH_VERSION=$(USPLASH_$(DISTRO_VERSION))



ifeq ($(HAVE_USPLASH),0)
DISABLE_USPLASH=1
endif



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
#DEB_MIRROR=http://ftp.uk.debian.org/debian/
DEB_MIRROR=http://192.168.0.3/mirror/debian/
endif

ifeq ($(DISTRO),ubuntu)
#DEB_MIRROR=http://archive.ubuntu.com/ubuntu/
DEB_MIRROR=http://192.168.0.3/mirror/ubuntu/
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
	@echo X11_CONF=$(X11_CONF)
	@echo
	@echo DISTRO=$(DISTRO)
	@echo DEB_MIRROR=$(DEB_MIRROR)
	@echo DISABLE_USPLASH=$(DISABLE_USPLASH)
	@echo "------------------------------------"

	@echo DISTRO_VERSION=$(DISTRO_VERSION)
	@echo TCOS_DEFAULT_KERNEL=$(TCOS_DEFAULT_KERNEL)
	@echo TCOS_USPLASH_VERSION=$(TCOS_USPLASH_VERSION)
