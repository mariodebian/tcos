all:
	#none

clean:
	rm -f *~ *glade.* *gladep*
	rm -rf po/es

install: es.gmo
	install -d $(DESTDIR)/$(PREFIX)/sbin
	install -m 755 tcos-standalone.py $(DESTDIR)/$(PREFIX)/sbin/tcos-standalone

	install -d $(DESTDIR)/$(PREFIX)/share/tcos-standalone
	install -d $(DESTDIR)/$(PREFIX)/share/tcos-standalone/images

	install -m 644 tcos-standalone.glade $(DESTDIR)/$(PREFIX)/share/tcos-standalone
	install -m 644 images/tcos-icon-32x32.png $(DESTDIR)/$(PREFIX)/share/tcos-standalone/images

	install -d $(DESTDIR)/$(PREFIX)/share/applications
	install -m 644 tcos-standalone.desktop $(DESTDIR)/$(PREFIX)/share/applications/

	# locales
	install -d $(DESTDIR)/$(PREFIX)/share/locale/es/LC_MESSAGES/
	install -m 644 po/es/LC_MESSAGES/tcos-standalone.mo $(DESTDIR)/$(PREFIX)/share/locale/es/LC_MESSAGES/tcos-standalone.mo


pot:
	xgettext  -o po/tcos-standalone.pot --files-from=po/FILES

es.po:
	msginit --input po/tcos-standalone.pot -o po/es-new.po
	msgmerge -o po/es-new.po po/es.po po/tcos-standalone.pot
	##################################################
	#           translate po/es-new.po               #
	##################################################

es.gmo:
	if [ -f po/es-new.po ]; then  mv po/es-new.po po/es.po ; fi
	mkdir -p po/es/LC_MESSAGES/
	msgfmt -o po/es/LC_MESSAGES/tcos-standalone.mo po/es.po


