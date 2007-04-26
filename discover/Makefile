###
###   Download discover packages and extract
###

DEB_MIRROR=http://idefix.eup.uva.es/debian/
#DEB_MIRROR=http://ftp.uk.debian.org/debian/

DISCOVER_FILES=$(shell apt-cache show discover libdiscover2 discover-data|awk '/^Filename/ {print "$(DEB_MIRROR)"$$2}')

all: test clean get uncompress
#	@make clean

test:
	@echo $(DISCOVER_FILES)

clean:
	@rm -rf *deb* *html* tmp/

dist-clean:
	@rm -f busybox
#	@rm *deb
#	@rm -rf tmp

get:
	@echo Downloading $(DISCOVER_FILES)
	@wget -q $(DISCOVER_FILES)

uncompress: clean dist-clean
	@echo "Extracting discover from deb files"
	@rm -rf tmp && mkdir -p tmp/data
	for file in *deb; do dpkg --extract $$file tmp/data ; done


install:
	install -d $(DESTDIR)/usr/lib/tcos/discover
	cp -ra tmp/data/* $(DESTDIR)/usr/lib/tcos/discover
	make clean
