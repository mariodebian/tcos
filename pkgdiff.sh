#!/bin/sh
#
# Simple script to track changes between installed tcos packages
# and compiled with "debuild -us -uc"
#
# This script show file changes between packages
#
#
PKGS=$(awk '/^Package:/ {print $2}' debian/control)

for PKG in $PKGS; do

  dpkg -L $PKG | sort | uniq | grep -v "^/.$" > pkg.____${PKG}____.instalado
  dpkg --contents ../${PKG}_*deb | \
	awk '{ if ($(NF-1) == "->") {print $(NF-2)} else {print $(NF)}}' |\
        sed -e 's/\.\//\//g' | sed -e 's/\/$//g' | grep -v "^$" | sort| uniq > pkg.____${PKG}____.generado

  diff -ur pkg.____${PKG}____.instalado pkg.____${PKG}____.generado

  rm -f pkg.____${PKG}____.instalado pkg.____${PKG}____.generado

done
