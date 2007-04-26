###
###   Download discover packages and extract
###



all: clean get uncompress


include ../common.mk
DISCOVER_FILES=$(shell apt-cache show discover libdiscover2 discover-data|awk '/^Filename/ {print "$(DEB_MIRROR)"$$2}')

test2:
	@echo $(DISCOVER_FILES)

clean:
	@rm -rf *deb* *html* tmp/

get:
	@echo Downloading $(DISCOVER_FILES)
	@wget -q $(DISCOVER_FILES)

uncompress: clean
	@echo "Extracting discover from deb files"
	@rm -rf tmp && mkdir -p tmp/data
	for file in *deb; do dpkg --extract $$file tmp/data ; done


install:
	install -d $(DESTDIR)/usr/lib/tcos/discover
	cp -ra tmp/data/* $(DESTDIR)/usr/lib/tcos/discover
	make clean
