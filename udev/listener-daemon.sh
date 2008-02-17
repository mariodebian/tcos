#!/bin/sh

#
# Launch /usr/sbin/mount-listener (see if /proc/mounts changes)
# and generate a udev mount/umount event



MOUNT_LISTENER=/usr/sbin/mount-listener
MOUNTS=/proc/mounts
LAST_MOUNTS=/tmp/proc_mounts



while [ 1 ]; do
  $MOUNT_LISTENER $MOUNTS $LAST_MOUNTS
done
