all:
	# nothing

clean:
	rm -f *~

install:
	install -m 755 session-cmd-send        $(DESTDIR)/${TCOS_BINS}/
	install -m 755 session-cmd-exec        $(DESTDIR)/${TCOS_BINS}/
	install -m 755 session-cmd-script      $(DESTDIR)/${TCOS_BINS}/
	install -m 644 session-cmd-common.sh   $(DESTDIR)/${TCOS_BINS}/

	mkdir -p $(DESTDIR)/etc/xdg/autostart/
	install -m 644 session-cmd.desktop     $(DESTDIR)/etc/xdg/autostart/
