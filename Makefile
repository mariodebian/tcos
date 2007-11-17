##
##     TCOS makefile
##
##    mariodebian \/at\/ gmail \/dot\/ com

all:
	if [ $(DISABLE_USPLASH) = 0 ]; then cd usplash && make; fi
	cd debootstrap && make
	#cd daemonize   && make
	cd printer     && make
	cd hex2ascii   && make
	cd xmlrpc      && make
	cd udev        && make
	cd lockscreen  && make
	cd dbus        && make
	cd session-cmd && make

include common.mk


clean:
	find * |grep "~" | xargs rm -rf --
	rm -f build-stamp configure-stamp
	cd usplash && make clean
	cd debootstrap && make clean
	#cd daemonize   && make clean
	cd printer     && make clean
	cd hex2ascii   && make clean
	cd xmlrpc      && make clean
	cd udev        && make clean
	cd lockscreen  && make clean
	cd dbus        && make clean
	cd session-cmd && make clean

gedit:
	gedit $(shell find bin/gentcos hooks-addons/ hooks/ scripts/ -type f|grep -v svn) >/dev/null 2>&1 &

boot_imgs:
	ppmtolss16 '#c0cfc0=7' < images/logo.ppm > tcos/logo.lss

distclean:
	find . -type f -name "*~" | xargs rm -rf --

config_svn:
	svn propedit svn:ignore .

#ubuntu-conf:
#	if [ "$(DISTRO)" = "ubuntu" ]; then\
#		sed -i 's/quiet splash/splash/g' $(DESTDIR)$(TCOS_CONF)/tcos.conf ;\
#		sed -i 's/quiet splash/splash/g' $(DESTDIR)$(TCOS_CONF)/pxelinux.cfg.tpl ;\
#		sed -i 's/quiet splash/splash/g' $(DESTDIR)$(TCOS_CONF)/menu.lst-tcos ;\
#	fi


install:
	#  Creating directories
	install -d $(DESTDIR)/$(TCOS_CONF)
	install -d $(DESTDIR)/$(TCOS_CONF)/hooks-addons/
	install -d $(DESTDIR)/$(TCOS_DIR)
	install -d $(DESTDIR)/$(TCOS_DIR)/hooks
	install -d $(DESTDIR)/$(TCOS_DIR)/hooks-addons
	install -d $(DESTDIR)/$(TCOS_DIR)/scripts
	install -d $(DESTDIR)/$(TCOS_DIR)/inc
	install -d $(DESTDIR)/$(TFTP_DIR)
	install -d $(DESTDIR)/$(TCOS_BINS)
	install -d $(DESTDIR)/$(X11_CONF)

	install -d $(DESTDIR)$(TCOS_CONF)/hacking
	install -d $(DESTDIR)/usr/sbin
	install -d $(DESTDIR)$(TFTP_DIR)/pxelinux.cfg
	install -d $(DESTDIR)$(TFTP_DIR)/conf/

	# usplash
	if [ $(DISABLE_USPLASH) = 0 ]; then cd usplash && make install DESTDIR=$(DESTDIR); fi

	install -d $(DESTDIR)$(TCOS_CONF)/hooks
	for i in `find scripts/ -type d`; do install -d $(DESTDIR)$(TCOS_CONF)/$$i; done
	
	for i in `find scripts/ -type d`; do install -d $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks/ -type d`; do install -d $(DESTDIR)$(TCOS_DIR)/$$i; done

	install -d $(DESTDIR)/var/lib/tcos/isos

	# FIXME this file isn't in Xorg 7.0 anymore???
	install -m 644 SecurityPolicy $(DESTDIR)/$(TCOS_CONF)

	install -m 644 tcos/default.tpl $(DESTDIR)$(TCOS_CONF)/pxelinux.cfg.tpl

	install -m 644 tcos/help.msg $(DESTDIR)$(TFTP_DIR)/
	install -m 644 tcos/help2.msg $(DESTDIR)$(TFTP_DIR)/
	install -m 644 tcos/tcos.msg $(DESTDIR)$(TFTP_DIR)/
	install -m 644 tcos/logo.lss $(DESTDIR)$(TFTP_DIR)/

	install -m 755 hooks/tcosmain $(DESTDIR)$(TCOS_CONF)/hooks/
	
	for i in `find scripts/ -type f`; do install -m 755 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks/ -type f`; do install -m 755 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks-addons/ -maxdepth 1 -type f`; do install -m 644 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done

	# delete tcosmain from initramfs-tools-tcos/hooks/
	rm -rf $(DESTDIR)$(TCOS_DIR)/hooks/tcosmain
	chmod -x $(DESTDIR)$(TCOS_DIR)/scripts/tcos


	install -m 600 conf/tcos.conf $(DESTDIR)$(TCOS_CONF)/tcos.conf
	sed -i 's/__TCOS_DEFAULT_KERNEL__/$(TCOS_DEFAULT_KERNEL)/g' $(DESTDIR)$(TCOS_CONF)/tcos.conf

	install -m 644 conf/version.conf $(DESTDIR)$(TCOS_CONF)/version.conf
	sed -i 's/__TCOS_VERSION__/$(VERSION)/g'       $(DESTDIR)$(TCOS_CONF)/version.conf
	sed -i 's/__TCOS_DISTRO__/$(DISTRO_VERSION)/g' $(DESTDIR)$(TCOS_CONF)/version.conf
	sed -i 's/__DISTRIBUTION__/$(DISTRO)/g' $(DESTDIR)$(TCOS_CONF)/version.conf
	sed -i 's/__TCOS_ARCH__/$(TCOS_ARCH)/g'        $(DESTDIR)$(TCOS_CONF)/version.conf

	# copy tcos.conf to have a default config
	install -m 644 $(DESTDIR)$(TCOS_CONF)/tcos.conf $(DESTDIR)$(TCOS_DIR)/tcos.conf

	install -m 644 conf/tcos-modules.conf $(DESTDIR)$(TCOS_CONF)/tcos-modules.conf

	install -m 644 conf/tcos-generation-functions.sh $(DESTDIR)$(TCOS_DIR)/tcos-generation-functions.sh
	install -m 644 conf/tcos-run-functions.sh        $(DESTDIR)$(TCOS_DIR)/tcos-run-functions.sh

	install -m 644 conf/82export_pulseaudio  $(DESTDIR)$(X11_CONF)/

	install -m 644 conf/initramfs.conf $(DESTDIR)$(TCOS_CONF)/initramfs.conf
	install -m 644 conf/template $(DESTDIR)$(TCOS_CONF)/hacking/template
	install -m 644 grub/menu.lst-tcos $(DESTDIR)$(TCOS_CONF)/menu.lst-tcos
	install -m 644 images/logo.xpm.gz $(DESTDIR)$(TCOS_CONF)/logo.xpm.gz

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
	install -m 755 bin/clear-logs      $(DESTDIR)/$(TCOS_BINS)/

	install -m 644 conf/xorg.conf.tpl $(DESTDIR)$(TCOS_CONF)/xorg.conf.tpl

	install -d $(DESTDIR)/usr/bin
	install -m 755 bin/tcos-shfsmount $(DESTDIR)$(TCOS_DIR)/inc/tcos-shfsmount
	install -m 755 bin/tcos-shfsumount $(DESTDIR)$(TCOS_DIR)/inc/tcos-shfsumount
	install -m 644 bin/shfscommon.sh $(DESTDIR)$(TCOS_DIR)/inc/shfscommon.sh

	cd debootstrap && make install TCOS_BINS=$(TCOS_BINS) DESTDIR=$(DESTDIR)
	#cd daemonize   && make install TCOS_BINS=$(TCOS_BINS) DESTDIR=$(DESTDIR)
	cd printer     && make install TCOS_BINS=$(TCOS_BINS) DESTDIR=$(DESTDIR)
	cd hex2ascii   && make install DESTDIR=$(DESTDIR)

	# xmlrpc
	cd xmlrpc && $(MAKE) install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)

	# udev
	cd udev && $(MAKE) install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR) TCOS_BINS=$(TCOS_BINS)

	# lockscreen
	cd lockscreen && $(MAKE) install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR) TCOS_BINS=$(TCOS_BINS) PACKAGE=$(PACKAGE)

	# dbus conf file
	cd dbus && $(MAKE) install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)

	# session-cmd scripts
	cd session-cmd && $(MAKE) install TCOS_BINS=$(TCOS_BINS) DESTDIR=$(DESTDIR)

targz: clean
	rm -rf ../tmp 2> /dev/null
	mkdir ../tmp
	cp -ra * ../tmp
	rm -rf `find ../tmp/* -type d -name .svn`
	mv ../tmp ../initramfs-tools-tcos-$(VERSION)
	tar -czf ../initramfs-tools-tcos-$(VERSION).tar.gz ../initramfs-tools-tcos-$(VERSION)
	rm -rf ../initramfs-tools-tcos-$(VERSION)

tcos: clean
	rm -f ../tcos_*deb ../gentcos*deb ../initramfs-tools-tcos*deb ../tcos_*deb ../tcos-server-utils*deb ../tcos-usplash*deb
	debuild -us -uc; true
	sudo dpkg -i ../initramfs-tools-tcos*deb ../gentcos*deb ../tcos-server-utils*deb ../tcos-usplash*deb ../tcos_*deb

patch_dapper:
	# PATCHING INITRAMFS_TOOLS_TCOS in Ubuntu DAPPER
	sed -i '/^Build/s/libusplash-dev/usplash, libbogl-dev, libgd-dev/g' debian/control	
	sed -i 's/source:Version/Source-Version/g' debian/control
	sed -i '/^Build/s/5.0.37.2/5.0.7ubuntu13/g' debian/control
	sed -i '/python-support/s/0.3/0.1.1ubuntu1/g' debian/control
	sed -i '/dh_pysupport/s/dh_pysupport/dh_python/g' debian/rules

patch_edgy:
	# PATCHING INITRAMFS_TOOLS_TCOS in Ubuntu EDGY
	sed -i '/^Build/s/libusplash-dev/usplash-dev, libbogl-dev/g' debian/control	

patch_feisty:
	# nothing to patch

patch_gutsy:
	# nothing to patch

patch_etch:
	# PATCHING INITRAMFS_TOOLS_TCOS in Debian etch
	sed -i '/^Build/s/libusplash-dev/usplash, libbogl-dev, libgd-dev/g' debian/control

patch_unstable:
	# nothing to patch


