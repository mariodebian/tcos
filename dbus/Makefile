DESTDIR:=/

all:
	@echo

include ../common.mk

install:
	install -d  $(DESTDIR)$(DBUS_CONF)/
	install -m 644 tcosmonitor.conf    $(DESTDIR)$(DBUS_CONF)/

clean:
	$(QUIET_CL)rm -f  *~
