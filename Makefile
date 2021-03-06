##
##     TCOS Makefile
##
##    mariodebian \/at\/ gmail \/dot\/ com
ARCH:=$(shell dpkg-architecture -qDEB_BUILD_ARCH)

all:
	$(MAKE) -C hex2ascii
	$(MAKE) -C xmlrpc
	$(MAKE) -C udev
	$(MAKE) -C lockscreen
	$(MAKE) -C tnc
	$(MAKE) -C dbus
	$(MAKE) -C session-cmd
	$(MAKE) -C tcos-standalone
	$(MAKE) -C tmixer

include common.mk


clean:
	@find * |grep "~" | xargs rm -rf --
	@rm -f build-stamp configure-stamp
	$(MAKE) -C hex2ascii  clean
	$(MAKE) -C xmlrpc     clean
	$(MAKE) -C udev       clean
	$(MAKE) -C lockscreen clean
	$(MAKE) -C tnc        clean
	$(MAKE) -C dbus       clean
	$(MAKE) -C session-cmd clean
	$(MAKE) -C tcos-standalone clean
	$(MAKE) -C tmixer      clean

distclean:
	find . -type f -name "*~" | xargs rm -rf --

install:
	#  Creating directories
	install -d $(DESTDIR)/$(TCOS_CONF)
	install -d $(DESTDIR)/$(TCOS_CONF)/ssl
	install -d $(DESTDIR)/$(TCOS_CONF)/hooks-addons/
	install -d $(DESTDIR)/$(TCOS_CONF)/conf.d/
	install -d $(DESTDIR)/$(TCOS_CONF)/secrets/
	install -d $(DESTDIR)/$(TCOS_DIR)
	install -d $(DESTDIR)/$(TCOS_DIR)/hooks
	install -d $(DESTDIR)/$(TCOS_DIR)/hooks-addons
	install -d $(DESTDIR)/$(TCOS_DIR)/scripts
	install -d $(DESTDIR)/$(TCOS_DIR)/inc
	install -d $(DESTDIR)/$(TFTP_DIR)
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -d $(DESTDIR)/$(X11_CONF)

	install -m 640 ssl/tcos_ca.crt       $(DESTDIR)$(TCOS_CONF)/ssl/
	install -m 640 ssl/tcos_dh1024.pem   $(DESTDIR)$(TCOS_CONF)/ssl/
	install -m 640 ssl/tcos_server.pem   $(DESTDIR)$(TCOS_CONF)/ssl/

	install -d $(DESTDIR)$(TCOS_CONF)/hacking
	install -d $(DESTDIR)$(TCOS_CONF)/pxelinux.d
	install -d $(DESTDIR)/usr/sbin
	install -d $(DESTDIR)/usr/bin
	install -d $(DESTDIR)$(TFTP_DIR)/pxelinux.cfg
	install -d $(DESTDIR)$(TFTP_DIR)/conf/

	install -d $(DESTDIR)$(TCOS_CONF)/hooks
	for i in `find scripts/ -type d`; do install -d $(DESTDIR)$(TCOS_CONF)/$$i; done
	
	for i in `find scripts/ -type d`; do install -d $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks/ -type d`; do install -d $(DESTDIR)$(TCOS_DIR)/$$i; done

	install -d $(DESTDIR)/var/lib/tcos/isos

	install -m 644 xorgids/xorg.pciids $(DESTDIR)/$(TCOS_DIR)/inc/

	install -m 644 tcos/default.tpl $(DESTDIR)$(TCOS_CONF)/pxelinux.cfg.tpl

	install -m 644 tcos/example.dual.server.conf $(DESTDIR)/$(TCOS_CONF)/pxelinux.d/example.dual.server.hook

	install -m 644 tcos/help.msg $(DESTDIR)$(TFTP_DIR)/
	install -m 644 tcos/help2.msg $(DESTDIR)$(TFTP_DIR)/
	install -m 644 tcos/tcos.msg $(DESTDIR)$(TFTP_DIR)/
	install -m 644 tcos/logo.lss $(DESTDIR)$(TFTP_DIR)/

	install -m 644 tcos/default.menu.tpl $(DESTDIR)$(TCOS_CONF)/pxelinux.menu.cfg.tpl
	install -m 644 tcos/pxetcos.jpg      $(DESTDIR)$(TFTP_DIR)/

	install -m 755 hooks/tcosmain $(DESTDIR)$(TCOS_CONF)/hooks/
	
	for i in `find scripts/ -type f`; do install -m 755 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks/ -type f`; do install -m 755 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks-addons/ -maxdepth 1 -type f`; do install -m 644 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done

	# delete tcosmain from tcos/hooks/
	rm -f $(DESTDIR)$(TCOS_DIR)/hooks/tcosmain
	chmod -x $(DESTDIR)$(TCOS_DIR)/scripts/tcos


	install -m 640 conf/tcos.conf $(DESTDIR)$(TCOS_CONF)/tcos.conf

	install -m 644 conf/version.conf $(DESTDIR)/var/lib/tcos/version.conf
	sed -i 's/__TCOS_VERSION__/$(VERSION)/g'       $(DESTDIR)/var/lib/tcos/version.conf
	sed -i 's/__TCOS_DISTRO__/$(DISTRO_VERSION)/g' $(DESTDIR)/var/lib/tcos/version.conf
	sed -i 's/__DISTRIBUTION__/$(DISTRO)/g'        $(DESTDIR)/var/lib/tcos/version.conf
	sed -i 's/__TCOS_ARCH__/$(TCOS_ARCH)/g'        $(DESTDIR)/var/lib/tcos/version.conf
	sed -i 's/__TCOS_MULTIARCH__/$(TCOS_MULTIARCH)/g'        $(DESTDIR)/var/lib/tcos/version.conf

	install -m 755 bin/gentcos                               $(DESTDIR)/usr/sbin/


	# tcos.conf templates
	install -d $(DESTDIR)$(TCOS_DIR)/templates/
	install -d $(DESTDIR)$(TCOS_CONF)/templates/
	install -m 644 conf/branding-pxelinux.info             $(DESTDIR)$(TCOS_CONF)/templates/

	# copy tcos.conf to have a default config
	install -m 644 conf/base.conf     $(DESTDIR)$(TCOS_DIR)/templates/base.conf
	install -m 644 conf/tcos.conf.all $(DESTDIR)$(TCOS_DIR)/templates/tcos.conf.all
	install -m 644 conf/tcos.conf.low $(DESTDIR)$(TCOS_DIR)/templates/tcos.conf.low
	install -m 644 conf/tcos.conf.nfs $(DESTDIR)$(TCOS_DIR)/templates/tcos.conf.nfs
	install -m 644 conf/tcos.conf.light $(DESTDIR)$(TCOS_DIR)/templates/tcos.conf.light


	install -m 644 conf/tcos-modules.conf $(DESTDIR)$(TCOS_DIR)/tcos-modules.conf

	install -m 644 conf/tcos-generation-functions.sh $(DESTDIR)$(TCOS_DIR)/tcos-generation-functions.sh
	install -m 644 conf/tcos-run-functions.sh        $(DESTDIR)$(TCOS_DIR)/tcos-run-functions.sh
	install -m 644 conf/branding.info                $(DESTDIR)$(TCOS_CONF)/conf.d/

	install -m 644 conf/80export_pulseaudio  $(DESTDIR)$(X11_CONF)/

	install -m 644 conf/initramfs.conf $(DESTDIR)$(TCOS_CONF)/initramfs.conf
	install -m 644 conf/template $(DESTDIR)$(TCOS_CONF)/hacking/template

	install -m 644 images/logo.xpm.gz $(DESTDIR)$(TCOS_DIR)/logo.xpm.gz

	# gentcos build script
	install -m 755 bin/gentcos            $(DESTDIR)/usr/sbin/gentcos
	install -m 755 bin/tcos-buildchroot   $(DESTDIR)/usr/sbin/tcos-buildchroot
	install -m 755 bin/tcos-gdm-autologin $(DESTDIR)/usr/sbin/tcos-gdm-autologin

	install -m 755 bin/configurexorg   $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/seq             $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/get_filesystem  $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/cd_type         $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/installer.sh    $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/tryXorg.sh      $(DESTDIR)/$(TCOS_BINS)/
	install -m 644 bin/set-limits      $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/daemonize.sh    $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/get_xauth       $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/clear-logs      $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/down-controller   $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/get_server        $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/clean_string.sh   $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/get_vga_vendor    $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/udhcpc-script     $(DESTDIR)/$(TCOS_BINS)/


	install -m 755 bin/tcos-bootchartd   $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/tcos-genbootchart $(DESTDIR)/usr/bin/tcos-genbootchart

	install -m 644 conf/xorg.conf.tpl $(DESTDIR)$(TCOS_CONF)/xorg.conf.tpl

	install -d $(DESTDIR)/usr/bin

	$(MAKE) -C hex2ascii install DESTDIR=$(DESTDIR)

	# xmlrpc
	$(MAKE) -C xmlrpc install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)
	sed -i 's/__VERSION__/$(VERSION)/g' $(DESTDIR)/$(TCOS_BINS)/tcos-last

	# udev
	$(MAKE) -C udev install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR) TCOS_BINS=$(TCOS_BINS)

	# lockscreen
	$(MAKE) -C lockscreen install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR) TCOS_BINS=$(TCOS_BINS) PACKAGE=$(PACKAGE)

	# tcos net controller
	$(MAKE) -C tnc install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR) TCOS_BINS=$(TCOS_BINS) PACKAGE=$(PACKAGE)

	# dbus conf file
	$(MAKE) -C dbus install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)

	# session-cmd scripts
	$(MAKE) -C session-cmd install TCOS_BINS=$(TCOS_BINS) DESTDIR=$(DESTDIR)

	# tcos-standalone helper
	$(MAKE) -C tcos-standalone install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)
	sed -i 's/__VERSION__/$(VERSION)/g' $(DESTDIR)/$(PREFIX)/sbin/tcos-standalone

	# tmixer
	$(MAKE) -C tmixer install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)

# VPN support not complete 
#	# openvpn
#	$(MAKE) -C openvpn install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)


