#!/bin/sh

#
# Launch /usr/sbin/mount-listener (see if /proc/mounts changes)
# and generate a udev mount/umount event
# only needed with kernel => 2.6.22
# 3106d46f51a1a72fdbf071ebc0800a9bcfcbc544
# patch-2.6.21-git3.log
#

# exit if kernel < 2.6.22
KMIN=$(uname -r | awk -F"." '{print $3}' | awk -F"-" '{print $1}')

if [ $KMIN -lt 22 ]; then
  exit 0
fi

MOUNT_LISTENER=/usr/sbin/mount-listener
MOUNTS=/proc/mounts
LAST_MOUNTS=/tmp/proc_mounts



while [ 1 ]; do
  $MOUNT_LISTENER $MOUNTS $LAST_MOUNTS
done
