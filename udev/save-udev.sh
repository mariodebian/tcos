#!/bin/sh

enable_debug=1
output_file=/tmp/tcos-udevd.log

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

[ $DEVNAME ] && [ -e $DEVNAME ] && export $(udevinfo --path=/sys$DEVPATH --query=env )

if [ "$(is_cdrom $PART)" = 1 ]; then
ID_FS_TYPE="cdfs"
fi

echo "ID_BUS=$ID_BUS#DEVNAME=$DEVNAME#ACTION=$ACTION#ID_FS_LABEL=$ID_FS_LABEL#ID_FS_TYPE=$ID_FS_TYPE#ID_VENDOR=$ID_VENDOR#ID_MODEL=$ID_MODEL#DEVPATH=$DEVPATH" >> $output_file
debug "ID_BUS=$ID_BUS#DEVNAME=$DEVNAME#ACTION=$ACTION#ID_FS_LABEL=$ID_FS_LABEL#ID_FS_TYPE=$ID_FS_TYPE#ID_VENDOR=$ID_VENDOR#ID_MODEL=$ID_MODEL#DEVPATH=$DEVPATH"



