#.SILENT:

PACKAGE=tcos

# uncomment for verbose build
#V=1

MAKE:=make -B
GCC:=gcc
INSTALL:=install
ifndef V
        QUIET_CC       = @echo '   $(TXT)'  CC    $@;
        QUIET_CL       = @echo '   $(TXT)'  CLEAN $@ `basename $(CURDIR)`;
        QUIET_LN       = @echo '   $(TXT)'  LINK  $@;
        MAKE           = @make --no-print-directory -B
endif

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
TCOS_STANDALONE_DIR=/var/lib/tcos/standalone


TCOS_XMLRPC_DIR=$(PREFIX)/share/tcos/xmlrpc/
DBUS_CONF=/etc/dbus-1/system.d/
X11_CONF=/etc/X11/Xsession.d/
OPENVPN_CONF=/var/lib/tcos/openvpn/

DISTRO_VERSION=$(shell dpkg-parsechangelog | awk '/^Distribution/ {print $$2}')
have_changelog := $(wildcard debian/changelog)
ifeq ($(strip $(have_changelog)),)
DISTRO_VERSION=$(shell dpkg-parsechangelog -l../debian/changelog | awk '/^Distribution/ {print $$2}')
endif

TCOS_ARCH:=$(shell dpkg-architecture  | awk -F"=" '/^DEB_BUILD_ARCH=/ {print $$2}')
TCOS_MULTIARCH:=$(shell dpkg-architecture -qDEB_HOST_MULTIARCH)



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


PREFIX:=/usr


test:
	@echo "------------------------------------"
	@echo VERSION=$(VERSION)
	@echo PACKAGE=$(PACKAGE)
	@echo 
	@echo TCOS_ARCH=$(TCOS_ARCH)
	@echo TCOS_MULTIARCH=$(TCOS_MULTIARCH)
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
	@echo OPENVPN_CONF=$(OPENVPN_CONF)
	@echo
	@echo DISTRO=$(DISTRO)
	@echo DISTRO_VERSION=$(DISTRO_VERSION)
	@echo
	@echo CFLAGS=$(CFLAGS)
	@echo LDFLAGS=$(LDFLAGS)
	@echo CPPFLAGS=$(CPPFLAGS)
	@echo CXXFLAGS=$(CXXFLAGS)
	@echo "------------------------------------"

