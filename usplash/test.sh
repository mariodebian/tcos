#!/bin/bash

# try to search usplash version

if [ ! -f /usr/include/usplash-theme.h ]; then
  USPLASH_VERSION="0.2"
  echo "${USPLASH_VERSION}"
  exit 0
fi



THEME_VERSION=$(awk '/^#define THEME_VERSION/ {print $3}'  /usr/include/usplash-theme.h)

USPLASH_VERSION=""

if [ $(ldd /sbin/usplash |grep -c libusplash) = 1 ]; then
	USPLASH_VERSION="0.4"
elif [ "$(THEME_VERSION)" = "1" ]; then \
	USPLASH_VERSION="0.2"
else
	USPLASH_VERSION="0.3"
fi


echo "${USPLASH_VERSION}"
