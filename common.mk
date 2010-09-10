#.SILENT:

PACKAGE=initramfs-tools-tcos

##################################
# kernel default versions

KERNEL_dapper="2.6.15-29-386"

KERNEL_edgy="2.6.17-12-generic"

KERNEL_feisty="2.6.20-17-generic"

KERNEL_etch="2.6.18-6-486"

KERNEL_lenny="2.6.26-2-486"

KERNEL_testing="2.6.32-5-486"

KERNEL_unstable="2.6.32-5-486"

KERNEL_experimental="2.6.32-5-486"

KERNEL_gutsy="2.6.22-16-generic"

KERNEL_max="2.6.24-24-generic"

KERNEL_hardy="2.6.24-28-generic"

KERNEL_intrepid="2.6.27-15-generic"

KERNEL_jaunty="2.6.28-19-generic"

KERNEL_karmic="2.6.31-22-generic"

KERNEL_lucid="2.6.32-24-generic"
##################################


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


TCOS_XMLRPC_DIR=$(PREFIX)/share/initramfs-tools-tcos/xmlrpc/
DBUS_CONF=/etc/dbus-1/system.d/
X11_CONF=/etc/X11/Xsession.d/

DISTRO_VERSION=$(shell dpkg-parsechangelog | awk '/^Distribution/ {print $$2}')
have_changelog := $(wildcard debian/changelog)
ifeq ($(strip $(have_changelog)),)
DISTRO_VERSION=$(shell dpkg-parsechangelog -l../debian/changelog | awk '/^Distribution/ {print $$2}')
endif

TCOS_ARCH:=$(shell dpkg-architecture  | awk -F"=" '/^DEB_BUILD_ARCH=/ {print $$2}')


TCOS_DEFAULT_KERNEL=$(KERNEL_$(DISTRO_VERSION))





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
	@echo "------------------------------------"

	@echo DISTRO_VERSION=$(DISTRO_VERSION)
	@echo TCOS_DEFAULT_KERNEL=$(TCOS_DEFAULT_KERNEL)
