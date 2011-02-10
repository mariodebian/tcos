#!/bin/sh
#
# Launch /usr/sbin/mount-listener (see if /proc/mounts changes)
# and generate a udev mount/umount event
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



MOUNT_LISTENER=/usr/sbin/mount-listener
MOUNTS=/proc/mounts
LAST_MOUNTS=/tmp/proc_mounts



while [ 1 ]; do
  $MOUNT_LISTENER $MOUNTS $LAST_MOUNTS
  echo "******* Something wrong happen mount-listener died ******* restarting..."
done
