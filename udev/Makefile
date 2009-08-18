all: mount-listener

include ../common.mk


mount-listener:
	$(QUIET_CC)$(GCC) -Wall -O2 -o mount-listener mount_listener.c


install:
	install -d  $(DESTDIR)$(TCOS_XMLRPC_DIR)
	install -d  $(DESTDIR)$(TCOS_BINS)

	install -m 755 tcos-udevd.sh          $(DESTDIR)$(TCOS_BINS)/
	install -m 755 mount-listener         $(DESTDIR)$(TCOS_BINS)/
	install -m 755 listener-daemon.sh     $(DESTDIR)$(TCOS_BINS)/
	install -m 755 save-udev.sh           $(DESTDIR)$(TCOS_BINS)/
	install -m 644 050_tcos_devices.rules $(DESTDIR)$(TCOS_XMLRPC_DIR)/

clean:
	$(QUIET_CL)rm -f *.pyc *~ mount-listener proc_mounts
