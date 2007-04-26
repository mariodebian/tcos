###
###   Download esound packages and extract
###



all: clean get uncompress


include ../common.mk
ESOUND_FILES=$(shell apt-cache show esound libesd0|awk '/^Filename/ {print "$(DEB_MIRROR)"$$2}')

test2:
	@echo $(ESOUND_FILES)

clean:
	@rm -rf *deb* *html* tmp/

get:
	@echo Downloading $(ESOUND_FILES)
	@wget -q $(ESOUND_FILES)

uncompress: clean
	@echo "Extracting esound from deb files"
	@rm -rf tmp && mkdir -p tmp/data
	for file in *deb; do dpkg --extract $$file tmp/data ; done


install:
	install -d $(DESTDIR)/usr/lib/tcos/esound
	cp -ra tmp/data/* $(DESTDIR)/usr/lib/tcos/esound
	make clean
