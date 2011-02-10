#!/bin/sh
#
# Simple script to track changes between installed tcos packages
# and compiled with "debuild -us -uc"
#
# This script show file changes between packages
#
#
# Copyright (C) 2006,2007,2008  mariodebian at gmail
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
