#!/bin/sh
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

OUTPUT=xorg.pciids
cat /dev/null > $OUTPUT


for i in /usr/share/xserver-xorg/pci/*ids openchrome.ids vboxvideo.ids; do

  driver=$(basename $i .ids)
  echo " * parsing driver $driver" >&2
  for _line in $(cat $i); do
    line=$(echo $_line | awk '{print substr($1,0,4)":"substr($1,5,8)}' | tr '[a-z]' '[A-Z]')
    echo "$driver $line" >> $OUTPUT
  done

done
