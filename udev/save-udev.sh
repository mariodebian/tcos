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

enable_debug=1
output_file=/tmp/tcos-udevd.log
UDEVINFO="/usr/bin/udevinfo"
[ -x /sbin/udevadm ] && UDEVINFO="/sbin/udevadm info"

debug() {
  [ $enable_debug = 1 ] && echo "  ** DEBUG (listener-daemon): $@" >&2
}

is_cdrom() {
  # receive hdc hda (without /dev/)
  cdrom=$(head -3 /proc/sys/dev/cdrom/info 2>/dev/null | tail -1 | cut -f 3- | grep -c "$1")
  echo $cdrom
}

#/dev/sda1 mount
DEVNAME=$1
ACTION=$2
debug "$@"

PART=$(echo "$DEVNAME" | awk -F"/" '{print $3}')
DISK=$(echo "$PART" | cut -c-3)
DEVPATH="/block/$DISK"
[ "$PART" != "$DISK" ] && DEVPATH="/block/$DISK/$PART"

[ -n "$DEVNAME" ] && [ -e $DEVNAME ] && export $($UDEVINFO --path=/sys$DEVPATH --query=env | grep -v "^DEVLINKS" | sed 's/[[:blank:]]/_/g')

if echo $DEVPATH | grep -q "/devices/" ; then
  # translate name
  _devpath=$(echo "$DEVPATH" | awk -F"/block" '{print "/block"$2}')
  debug "save-udev.sh OLD ==>$DEVPATH"
  debug "save-udev.sh NEW ==>$_devpath"
  DEVPATH=$_devpath
fi

if [ "$ID_FS_LABEL_SAFE" ]; then
  ID_FS_LABEL=$ID_FS_LABEL_SAFE
fi

if [ "$(is_cdrom $PART)" = 1 ]; then
ID_FS_TYPE=$(cd_type $DEVNAME)
fi

if [ -z $ID_VENDOR ]; then
  ID_VENDOR=$(cat $(dirname $(find /sys/class/scsi_disk/*/device/  -name "block:$DISK" 2>/dev/null) 2>/dev/null)/vendor 2>/dev/null | sed 's/ //g')
fi

if [ -z $ID_MODEL ]; then
  ID_MODEL=$(cat $(dirname $(find /sys/class/scsi_disk/*/device/  -name "block:$DISK" 2>/dev/null) 2>/dev/null)/model 2>/dev/null | sed 's/ //g')
fi

echo "ID_BUS=$ID_BUS#DEVNAME=$DEVNAME#ACTION=$ACTION#ID_FS_LABEL=$ID_FS_LABEL#ID_FS_TYPE=$ID_FS_TYPE#ID_VENDOR=$ID_VENDOR#ID_MODEL=$ID_MODEL#DEVPATH=$DEVPATH" >> $output_file
debug "ID_BUS=$ID_BUS#DEVNAME=$DEVNAME#ACTION=$ACTION#ID_FS_LABEL=$ID_FS_LABEL#ID_FS_TYPE=$ID_FS_TYPE#ID_VENDOR=$ID_VENDOR#ID_MODEL=$ID_MODEL#DEVPATH=$DEVPATH"


exit 0
