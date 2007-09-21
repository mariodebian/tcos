#!/bin/sh

#
# Launch listener_mounter (see if /proc/mounts changes)
# and generate a udev mount/umount event
# only needed with kernel => 2.6.22
# 3106d46f51a1a72fdbf071ebc0800a9bcfcbc544
# patch-2.6.21-git3.log
#

# exit if kernel < 2.6.22
KMAY=$(uname -r | awk -F"." '{print $1}')
KMED=$(uname -r | awk -F"." '{print $2}')
KMIN=$(uname -r | awk -F"." '{print $3}' | awk -F"-" '{print $1}')

if [ $KMIN -lt 22 ]; then
  exit 0
fi

enable_debug=1
MOUNT_LISTENER=/usr/sbin/mount-listener
output_file=/tmp/tcos-udevd.log
MOUNTS=/proc/mounts
LAST_MOUNTS=/tmp/proc_mounts

debug() {
  [ $enable_debug = 1 ] && echo "  ** DEBUG: $@" >&2
}

restore(){
  cat $MOUNTS > $LAST_MOUNTS
}


is_cdrom() {
  # receive hdc hda (without /dev/)
  cdrom=$(head -3 /proc/sys/dev/cdrom/info 2>/dev/null | tail -1 | cut -f 3- | grep -c $1)
  echo $cdrom
}


do_action() {
  #/dev/sda1 mount
  DEVNAME=$1
  ACTION=$2
  debug "$@"

  PART=$(echo "$DEVNAME" | awk -F"/" '{print $3}')
  DISK=$(echo "$PART" | cut -c-3)
  DEVPATH="/block/$DISK"
  [ "$PART" != "$DISK" ] && DEVPATH="/block/$DISK/$PART"

  
  export $(udevinfo --path=/sys$DEVPATH --query=env )
  
  if [ "$(is_cdrom $PART)" = 1 ]; then
    ID_FS_TYPE="cdfs"
  fi
  
  echo "ID_BUS=$ID_BUS#DEVNAME=$DEVNAME#ACTION=$ACTION#ID_FS_LABEL=$ID_FS_LABEL#ID_FS_TYPE=$ID_FS_TYPE#ID_VENDOR=$ID_VENDOR#ID_MODEL=$ID_MODEL#DEVPATH=$DEVPATH" >> $output_file
  debug "ID_BUS=$ID_BUS#DEVNAME=$DEVNAME#ACTION=$ACTION#ID_FS_LABEL=$ID_FS_LABEL#ID_FS_TYPE=$ID_FS_TYPE#ID_VENDOR=$ID_VENDOR#ID_MODEL=$ID_MODEL#DEVPATH=$DEVPATH"

  restore
}

if [ ! -x $MOUNT_LISTENER ]; then
   debug "$MOUNT_LISTENER not found"
   exit 1
fi

restore
while [ 1 ]; do
  do_action $($MOUNT_LISTENER $MOUNTS $LAST_MOUNTS)
done
