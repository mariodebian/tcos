#!/bin/bash

#
#
#   #define THEME_VERSION 2
#
#

if [ -f /usr/include/usplash-theme.h ]; then
  awk '/^#define THEME_VERSION/ {print $3}'  /usr/include/usplash-theme.h
else
  awk '/^#define THEME_VERSION/ {print $3}'  usplash-theme.h
fi
