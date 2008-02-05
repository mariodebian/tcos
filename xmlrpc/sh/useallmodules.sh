#!/bin/sh

#/*
#* useallmodules.sh shell script to download allmodules-$(uname -r).squashfs
#*                  and mount in /lib/modules/$(uname -r)/
#* Copyright (C) 2006,2007,2008  mariodebian at gmail
#*
#* This program is free software; you can redistribute it and/or
#* modify it under the terms of the GNU General Public License
#* as published by the Free Software Foundation; either version 2
#* of the License, or (at your option) any later version.
#*
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU General Public License for more details.
#*
#* You should have received a copy of the GNU General Public License
#* along with this program; if not, write to the Free Software
#* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#*/


if [ ! -e /conf/tcos.conf ]; then
   echo -n "error running in standalone"
   exit 1
fi

[ -f /tmp/allmodules_done ] && echo "OK"; exit 0

touch /tmp/allmodules_done

. /scripts/functions

. /conf/tcos.conf
. /conf/tcos-run-functions

allmodules=/mnt/tmp/allmodules.squashfs

download_file /tcos/allmodules-$(uname -r).squashfs ${allmodules}
if [ "$(if_is_zero ${allmodules})" = "0" ]; then
   echo "error: allmodules-$(uname -r).squashfs not found, generate it with gentcos -tftp -allmodules"
   exit 0
fi


# mount
mount -r -o loop -t squashfs ${allmodules} /lib/modules/$(uname -r)/

# remount
mount_unionfs /mnt/.ram-modules /mnt/.modules /lib/modules/$(uname -r)

# reload modules....
if [ -x /usr/bin/discover-modprobe ]; then
  discover-modprobe -nv >/dev/null 2>&1
fi

echo "OK"
exit 0
