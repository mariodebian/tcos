all:

include ../common.mk



install:
	install -d $(DESTDIR)$(OPENVPN_CONF)
	install -m 644 openssl.conf         $(DESTDIR)$(OPENVPN_CONF)/
	install -m 644 tcosserver.conf      $(DESTDIR)$(OPENVPN_CONF)/
	install -m 755 tcos-init-openvpn    $(DESTDIR)/usr/sbin/
