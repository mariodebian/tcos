#!/bin/sh
#/*
#* tcos-pam-usb.sh
#* Copyright (C) 2008  mariodebian at gmail
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

#
#  TCOS pam-usb helper
#

ALLOW_NOT_REMOVABLE=1

export DISPLAY=:0

if [ -e /conf/tcos-run-functions ]; then
  # running in thin client
  STANDALONE=0
  export XAUTHORITY=/root/.Xauthority
else
  echo "error: tcos-pam-usb not allowed in standalone"
  exit 1
fi

mount_uuid() {
  if [ ! -e /dev/disk/by-uuid/$1 ]; then
     echo "error: device UUID=$1 not found"
     return
  fi
  # create mount dir
  [ ! -d /mnt/$1 ] && mkdir /mnt/$1

  # umount if mounted
  grep -q /mnt/$1 /proc/mounts && umount /mnt/$1

  mount -t$(get_filesystem /dev/disk/by-uuid/$1 --only) /dev/disk/by-uuid/$1 /mnt/$1
  if [ $? = 0 ]; then
     echo "ok"
  else
     echo "error: mounting device UUID=$1"
  fi
  cat /dev/null > /tmp/tcos-udevd.log
}

umount_uuid() {
  grep -q "$1" /proc/mounts || return
  umount /mnt/$1 >> /tmp/initramfs.debug 2>&1
  cat /dev/null > /tmp/tcos-udevd.log
}


for arg in $1; do
  case $arg in
     initusb)
         if [ -x /bin/ltspfs-set-xprop ]; then
  		/bin/ltspfs-set-xprop now 2>&1 >> /tmp/initramfs.debug
	 fi
	 echo "ok"
     ;;
     mountusb)
         mount_uuid $2
     ;;
     checkdevice)
         UUID=""
         # receive something like this: UUID=C0C3-B657#MODEL=Flash Disk#VENDOR=ORP#SERIAL=ORP_Flash_Disk_293424206251-0:0
         data=$(echo "$2" | sed 's/#/\n/g' | sed 's/ /_/g')
         for line in $data; do 
           export $line 
         done

         if [ "$UUID" = "" ]; then
             echo "error: UUID not set"
             exit 0
         fi

         DEVICE=$(basename $(readlink /dev/disk/by-uuid/$UUID))

         if [ "$MODEL" != "$(udevinfo --query=env --name=$DEVICE| awk -F"=" '/^ID_MODEL=/ {print $2}')" ]; then
             echo "error: MODEL not match"
             exit 0
         fi

         if [ "$VENDOR" != "$(udevinfo --query=env --name=$DEVICE| awk -F"=" '/^ID_VENDOR=/ {print $2}')" ]; then
             echo "error: VENDOR not match"
             exit 0
         fi

         if [ "$SERIAL" != "$(udevinfo --query=env --name=$DEVICE| awk -F"=" '/^ID_SERIAL=/ {print $2}')" ]; then
             echo "error: SERIAL not match"
             exit 0
         fi


         echo "ok"
     ;;
     umountusb)
         umount_uuid $2
         echo "ok"
     ;;
     checkuuid)
         if [ ! -e "/dev/disk/by-uuid/$2" ]; then
           echo "off"
         else
           echo "on"
         fi
     ;;
     *)
       echo "error: arguments error"
     ;;
  esac

done
