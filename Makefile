#VERSION=0.52
VERSION=$(shell head -1 debian/changelog | awk  '{gsub(/\(/,"",$$2); gsub(/\)/, "" , $$2); print $$2}' )

#TCOS_DIR=/usr/share/initramfs-tools-tcos
TCOS_DIR=$(shell awk -F "=" '/TCOS_DIR=/ {print $$2}' conf/tcos.conf )

#TFTP_DIR=/tftpboot/tcos
TFTP_DIR=$(shell awk -F "=" '/TFTP_DIR=/ {print $$2}' conf/tcos.conf )

#TCOS_CONF=/etc/tcos
TCOS_CONF=$(shell awk -F "=" '/TCOS_CONF=/ {print $$2}' conf/tcos.conf )

TCOS_BINS=$(shell awk -F "=" '/TCOS_BINS=/ {print $$2}' conf/tcos.conf )


all:
	cd usplash && make
	cd debootstrap && make
	cd discover && make
	# nothing more to compile, => make install

test:
	@echo VERSION=$(VERSION)
	@echo TCOS_CONF=$(TCOS_CONF)
	@echo TCOS_DIR=$(TCOS_DIR)
	@echo TFTP_DIR=$(TFTP_DIR)
	@echo TCOS_BINS=$(TCOS_BINS)

clean:
	find * |grep "~" | xargs rm -rf --
	if [ -d debian/initramfs-tools-tcos ]; then rm -rf debian/initramfs-tools-tcos; fi
	if [ -d debian/gentcos ] ; then rm -rf debian/gentcos; fi
	if [ -d debian/tcos-server-utils ] ; then rm -rf debian/tcos-server-utils; fi
	if [ -d debian/tcos-usplash ] ; then rm -rf debian/tcos-usplash; fi
	if [ -d debian/tcos-discover2 ] ; then rm -rf debian/tcos-discover2; fi
	if [ -d debian/tmp ] ; then rm -rf debian/tmp; fi
	rm -f build-stamp configure-stamp
	cd usplash && make clean
	cd debootstrap && make clean

gedit:
	gedit-cvs $(shell find bin/gentcos hooks-addons/ hooks/ scripts/ -type f|grep -v svn) >/dev/null 2>&1 &

boot_imgs:
	ppmtolss16 '#c0cfc0=7' < images/logo.ppm > tcos/logo.lss
#	ppmtolss16 '#000000=7' < images/logo.ppm > tcos/logo.lss

distclean:
	find * |grep "~" | xargs rm -rf --

config_svn:
	svn propedit svn:ignore .


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

	install -d $(DESTDIR)$(TCOS_CONF)/hacking
	install -d $(DESTDIR)/usr/sbin
	install -d $(DESTDIR)$(TFTP_DIR)/pxelinux.cfg
	install -d $(DESTDIR)$(TFTP_DIR)/conf/

	# usplash
	install -d $(DESTDIR)/usr/lib/usplash/
	install -m 755 usplash/usplash-tcos.so $(DESTDIR)/usr/lib/usplash/usplash-tcos.so

	install -d $(DESTDIR)$(TCOS_CONF)/hooks
	for i in `find scripts/ -type d`; do install -d $(DESTDIR)$(TCOS_CONF)/$$i; done
	
#	for i in `find scripts/ -type d`; do install -d $(DESTDIR)/etc/tcos/$$i; done
#	for i in `find hooks/ -type d`; do install -d $(DESTDIR)/etc/tcos/$$i; done

	for i in `find scripts/ -type d`; do install -d $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks/ -type d`; do install -d $(DESTDIR)$(TCOS_DIR)/$$i; done

#FIXME change boot/grub to /etc/tcos
#generate iso in current dir or ask for it
#	install -d $(DESTDIR)/var/lib/tcos/master/boot/grub
	install -d $(DESTDIR)/var/lib/tcos/isos

	# FIXME this file isn't in Xorg 7.0 anymore???
	install -m 644 SecurityPolicy $(DESTDIR)/$(TCOS_CONF)



	install -m 644 tcos/default $(DESTDIR)$(TFTP_DIR)/pxelinux.cfg/
	install -m 644 tcos/help.msg $(DESTDIR)$(TFTP_DIR)/
	install -m 644 tcos/tcos.msg $(DESTDIR)$(TFTP_DIR)/
	install -m 644 tcos/logo.lss $(DESTDIR)$(TFTP_DIR)/

	# Installing in  $(DESTDIR)
#	for i in `find scripts/ -type f`; do install -m 755 $$i $(DESTDIR)/etc/tcos/$$i; done
#	for i in `find hooks/ -type f`; do install -m 755 $$i $(DESTDIR)/etc/tcos/$$i; done
#	for i in `find hooks-addons/ -maxdepth 1 -type f`; do install -m 644 $$i $(DESTDIR)/etc/tcos/$$i; done

	install -m 755 hooks/tcosmain $(DESTDIR)$(TCOS_CONF)/hooks/
	
	for i in `find scripts/ -type f`; do install -m 755 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks/ -type f`; do install -m 755 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done
	for i in `find hooks-addons/ -maxdepth 1 -type f`; do install -m 644 $$i $(DESTDIR)$(TCOS_DIR)/$$i; done

	# delete tcosmain
	rm -rf $(DESTDIR)$(TCOS_DIR)/hooks/tcosmain
	chmod -x $(DESTDIR)$(TCOS_DIR)/scripts/tcos


	install -m 644 conf/tcos.conf $(DESTDIR)$(TCOS_CONF)/tcos.conf
	install -m 644 conf/tcos-modules.conf $(DESTDIR)$(TCOS_CONF)/tcos-modules.conf

	install -m 644 conf/tcos-generation-functions.sh $(DESTDIR)$(TCOS_DIR)/tcos-generation-functions.sh
	install -m 644 conf/tcos-run-functions.sh        $(DESTDIR)$(TCOS_DIR)/tcos-run-functions.sh

	install -m 644 conf/initramfs.conf $(DESTDIR)$(TCOS_CONF)/initramfs.conf
	install -m 644 conf/template $(DESTDIR)$(TCOS_CONF)/hacking/template
	install -m 644 grub/menu.lst-tcos $(DESTDIR)$(TCOS_CONF)/menu.lst-tcos
	
	# gentcos build script
	install -m 755 bin/gentcos $(DESTDIR)/usr/sbin/gentcos

#	install -d $(DESTDIR)/usr/share/initramfs-tools-tcos/
#	install -m 755 bin/configurexorg $(DESTDIR)/usr/share/initramfs-tools-tcos/configurexorg
	install -m 755 bin/configurexorg $(DESTDIR)/$(TCOS_BINS)/
	install -m 755 bin/installer.sh $(DESTDIR)/$(TCOS_BINS)/

	install -m 644 conf/xorg.conf.tpl $(DESTDIR)$(TCOS_CONF)/xorg.conf.tpl

	install -d $(DESTDIR)/usr/bin
	install -m 755 bin/tcos-shfsmount $(DESTDIR)$(TCOS_DIR)/inc/tcos-shfsmount
	install -m 755 bin/tcos-shfsumount $(DESTDIR)$(TCOS_DIR)/inc/tcos-shfsumount
	install -m 644 bin/shfscommon.sh $(DESTDIR)$(TCOS_DIR)/inc/shfscommon.sh

	cd debootstrap && make install TCOS_BINS=$(TCOS_BINS) DESTDIR=$(DESTDIR)

	cd discover && make install DESTDIR=$(DESTDIR)


targz: clean
	rm -rf ../tmp 2> /dev/null
	mkdir ../tmp
	cp -ra * ../tmp
	###################
	# Borrando svn... #
	###################
	rm -rf `find ../tmp/* -type d -name .svn`
	mv ../tmp ../initramfs-tools-tcos-$(VERSION)
	tar -czf ../initramfs-tools-tcos-$(VERSION).tar.gz ../initramfs-tools-tcos-$(VERSION)
	rm -rf ../initramfs-tools-tcos-$(VERSION)

tcos: clean
	rm -f ../initramfs ../usr.squashfs ../tcos_*deb ../gentcos*deb ../initramfs-tools-tcos*deb ../tcos-server-utils*deb ../tcos-usplash*deb
	debuild -us -uc; true
	sudo dpkg -i ../initramfs-tools-tcos*deb ../gentcos*deb ../tcos-server-utils*deb ../tcos-usplash*deb ../tcos_*deb
