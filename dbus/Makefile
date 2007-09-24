DESTDIR:=/

all:
	

include ../common.mk

install:
	install -d  $(DESTDIR)$(DBUS_CONF)/
	install -m 644 tcosmonitor.conf    $(DESTDIR)$(DBUS_CONF)/

clean:
	rm -f  *~
