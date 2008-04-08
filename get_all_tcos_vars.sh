#!/bin/bash

ALL_VARS=$(rgrep "\$TCOS_" * |grep -v svn | awk -F'TCOS_' '{print "TCOS_"$2}' | \
 awk '{print $1}' | awk -F"=" '{print $1}' | awk -F"/" '{print $1}' | \
 sed 's/)//g' | sed 's/}//g' | sed 's/"//g'| sed 's/;//g' |sort | uniq)

for VAR in $ALL_VARS; do
  base=$(grep -c "^${VAR}=" conf/base.conf)
  tcos=$(grep -c "^${VAR}=" conf/tcos.conf)
  echo "$VAR base.conf=$base tcos.conf=$tcos"
done
