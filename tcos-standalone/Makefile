LANGUAGES=es ca@valencia pt_BR qcv

all: gmo

include ../common.mk

clean:
	$(QUIET_CL)rm -f *~ *glade.* *gladep*
	@for lang in $(LANGUAGES); do \
		echo "      **CLEANDIR** po/$$lang"; \
		rm -rf po/$$lang;\
	done

install:
	install -d $(DESTDIR)/$(PREFIX)/sbin
	install -m 755 tcos-standalone.py $(DESTDIR)/$(PREFIX)/sbin/tcos-standalone

	install -d $(DESTDIR)/$(PREFIX)/share/tcos-standalone
	install -d $(DESTDIR)/$(PREFIX)/share/tcos-standalone/images

	install -m 644 tcos-standalone.glade $(DESTDIR)/$(PREFIX)/share/tcos-standalone
	install -m 644 images/tcos-icon-32x32.png $(DESTDIR)/$(PREFIX)/share/tcos-standalone/images

	install -d $(DESTDIR)/$(PREFIX)/share/applications
	install -m 644 tcos-standalone.desktop $(DESTDIR)/$(PREFIX)/share/applications/

	# locales
	@for lang in $(LANGUAGES); do \
		install -d $(DESTDIR)/$(PREFIX)/share/locale/$$lang/LC_MESSAGES/; \
		install -m 644 po/$$lang/LC_MESSAGES/tcos-standalone.mo $(DESTDIR)/$(PREFIX)/share/locale/$$lang/LC_MESSAGES/tcos-standalone.mo; \
	done


pot:
	xgettext  -o po/tcos-standalone.pot --files-from=po/FILES


po:
	@for lang in $(LANGUAGES); do \
		echo " **PO** po/$$lang"; \
		if [ ! -f po/$$lang.po ]; then \
			msginit --input po/tcos-standalone.pot -o po/$$lang-old.po;\
		else \
			mv po/$$lang.po po/$$lang-old.po; \
		fi ; \
		msgmerge -o po/$$lang.po po/$$lang-old.po po/tcos-standalone.pot; \
		rm -f po/$$lang-old.po; \
	done

gmo:
	@echo " **COMPILING** languages: $(LANGUAGES)"
	@for lang in $(LANGUAGES); do \
		echo " **COMPILEMO** po/$$lang" ; \
		mkdir -p po/$$lang/LC_MESSAGES/; \
		msgfmt --verbose -o po/$$lang/LC_MESSAGES/tcos-standalone.mo po/$$lang.po; \
	done

