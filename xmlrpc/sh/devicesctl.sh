#!/bin/sh
#/*
#* devices.ctl.sh Devices remote control
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


if [ -e /conf/tcos-run-functions ]; then
  . /conf/tcos-run-functions
else
  echo -n "error running in standalone"
  exit 1
fi


tmp_file=/tmp/devicesctl

output=""
need_parse="0"

LANG=C
FDISK="/sbin/fdisk"

read_line() {
  head -$1 $tmp_file | tail -1
}

get_fs_type() {
  output=$(get_filesystem "$1" --only)
  echo $output
}

is_cdrom() {
  # receive hdc hda (without /dev/)
  cdrom=$(head -3 /proc/sys/dev/cdrom/info 2>/dev/null | tail -1 | cut -f 3- | grep -c $1)
  echo $cdrom
}


if [ "$1" = "--showlocaldisks" ]; then
 #output=$($FDISK -l | awk '/^\/dev/ {print $1}')
 output=$(ls /sys/block/|grep -v ram|grep -v loop)
 need_parse="1"
fi

if [ "$1" = "--getsize" ]; then
  if [ "$2" != "" ]; then
    output=$($FDISK -l /dev/$2 | grep "/dev/$2:" | awk  '{print $3" "$4}' | sed s/,//g)
  else
    output="error: need a device!!"
  fi
fi


if [ "$1" = "--getparts" ]; then
  if [ "$2" != "" ]; then
    allparts=$(echo /sys/block/$2/$2*)
    for part in $allparts; do echo $(basename $part) >> $tmp_file ;done
    need_parse=1
    output="tmp_file"
  else
    output="error: need a device!!"
  fi
fi

if [ "$1" = "--gettype" ]; then
  if [ "$2" != "" ]; then
    output=$(get_fs_type $2)
  else
    output="error: need a device!!"
  fi
fi

if [ "$1" = "--getdmesg" ]; then
  output=$(dmesg|tail)
  need_parse=1
fi


if [ "$1" = "--getudev" ]; then
#  if [ -e /tmp/gnetic.log ]; then
#     if [ "$(pidof gnetic | sed '/^$/d')" = "" ]; then
#       if [ "$(cat /tmp/gnetic.log | grep -c Error)" != "0" -o "$(cat /tmp/gnetic.log | grep -c Killed)" != "0" -o "$(cat /tmp/gnetic.log | grep -c #Terminated)" != "0" ]; then
#          output="clone-failed"
#       elif [ "$(cat /tmp/gnetic.log | grep -c FINISH)" != "0" ]; then
#          output="cloned"
#       fi
#     else
#       output="cloning"
#     fi
#     need_parse="0"
#  else
   output=$(cat /tmp/tcos-udevd.log 2>/dev/null)
   cat /dev/null > /tmp/tcos-udevd.log
   need_parse=1
#  fi
fi


if [ "$1" = "--mount" ]; then
  [ -x /bin/ltspfs-set-xprop ] && /bin/ltspfs-set-xprop now 2>&1 >> /tmp/initramfs.debug
  if [ "$2" != "" ]; then
      mnt=$(basename $2)
      if [ $(grep -c /mnt/$mnt /proc/mounts) != 0 ]; then
        echo -n "/mnt/$mnt"
        return
      fi
      cdrom=$(is_cdrom $mnt)
      fs=""
      if [ "$3" != "" ]; then
         fs=" -t $3 "
      else
         fs=" -t $(get_fs_type $2) "
      fi
      mkdir -p /mnt/$mnt
      if [ "$cdrom" = 1 ]; then
        mount -t $(cd_type /dev/$mnt) -o ro $2 /mnt/$mnt 2> /dev/null
        if [ $? = 0 ]; then
          output="/mnt/$mnt"
        else
          output="error: mounting device"
        fi
        echo -n $output
        return
      fi
      ###############
      count=0
      if [ "$(get_fs_type $2)" = "ntfs-3g" ]; then
        /sbin/mount.ntfs-3g $2 /mnt/$mnt
         ln -s /sbin/mount.ntfs-3g /sbin/mount.ntfs-3g.$mnt 2>/dev/null
        /sbin/start-stop-daemon --quiet --background --start --startas /sbin/mount.ntfs-3g.$mnt --name /sbin/mount.ntfs-3g.$mnt -- $2 /mnt/$mnt
        while [ $(grep -c "^$2" /proc/mounts) -eq 0 -a $count -lt 5 ];do
           sleep 1
           count=$(($count+1))
        done
      else
        mount $fs $2 /mnt/$mnt 2>/dev/null
      fi
      if [ $? = 0 -a $count -ne 5 ]; then
        output="/mnt/$mnt"
      else
        output="error: mounting device"
      fi
  else
      output="error: need a device"
  fi
fi


if [ "$1" = "--umount" ]; then
  if [ "$2" != "" ]; then
    mnt=$(basename $2)
    umount /mnt/$mnt 2>/dev/null
    if [ $? = 0 ]; then
      [ -e /sbin/mount.ntfs-3g.$mnt ] && rm -f /sbin/mount.ntfs-3g.$mnt 2>/dev/null
      output="/mnt/$mnt"
    else
      output="error: umounting device"
    fi
  else
    output="error: need something to umount"
  fi
fi


if [ "$1" = "--getstatus" ]; then
 if [ "$2" != "" ]; then
   output=$(grep -c "^$2 " /proc/mounts)
 else
   output="error: need a device"
 fi

fi

if [ "$1" = "--eject" ]; then
 if [ "$2" != "" ]; then
   eject /dev/$2
 else
   eject
 fi
 echo "ok"

fi


if [ "$1" = "--getcdrom" ]; then
  cdrom=$(head -3 /proc/sys/dev/cdrom/info 2>/dev/null | tail -1 | cut -f 3-)
  for item in $cdrom; do output="$output$item|"; done
  need_parse=0
fi

if [ "$1" = "--cdaudio" ]; then
  type=$(cd_type $2)
  if [ "$type" = "cdfs" ]; then
    output=1
  else
    output=0
  fi
  need_parse=0
fi

if [ "$1" = "--getxdrivers" ]; then
  if [ -d /usr/lib/xorg/modules/drivers/ ]; then
    output=$(ls /usr/lib/xorg/modules/drivers/|grep "_drv.so"|sed s/'_drv.so'//g)
  fi
  need_parse=1
fi

if [ "$1" = "--exists" ]; then
  if [ -e "$2" ]; then
    output=1
  else
    output=0
  fi
  need_parse=0
fi

if [ "$1" = "--gethdd" ]; then
  hdd=$(grep ^/dev /etc/fstab | grep -v -e swap -e iso9660 | awk '{print $1}' | sed 's/\/dev\///g')
  for item in $hdd; do output="$output$item|"; done
  need_parse=0
fi

if [ "$1" = "--getid" ]; then
  output=$(udevinfo --query=env --name=$2| grep -e "^ID_VENDOR=" -e "^ID_MODEL="| awk -F"=" '{print $2}' 2>/dev/null)
  need_parse=1
fi


if [ "$1" = "--getusbip" ]; then
  output=$(/usr/bin/usbip_list 2>/dev/null)
  need_parse=1
fi

usage() {
  echo "Usage:"
  echo "       $0  --help                  ( this help text )"
  echo "       $0  --showlocaldisks        ( return all devices )"
  echo "       $0  --getsize  DEVICE       ( return size of DEVICE )"
  echo "       $0  --getparts DEVICE       ( return partittions of DEVICE )"
  echo "       $0  --gettype DEVICE        ( return DEVICE type )"
  echo "       $0  --getudev               ( return udev block events log )"
  echo "       $0  --getdmesg              ( return last lines of dmesg )"
  echo "       $0  --mount  DEVICE         ( mount DEVICE )"
  echo "       $0  --umount  DEVICE        ( umount DEVICE )"
  echo "       $0  --getstatus DEVICE      ( return 1 if mounted or 0 if not mounted )"
  echo "       $0  --eject                 ( eject cdrom device )"
  echo "       $0  --getcdrom              ( return all cdrom devices )"
  echo "       $0  --cdaudio  DEVICE       ( return 1 if cdaudio, 0 if not )"
  echo "       $0  --getxdrivers           ( return all xorg drivers found )"
  echo "       $0  --exists  ARG           ( return 1 if exists 0 if not )"
  echo "       $0  --gethdd                ( return all hard disk partitions )"
  echo "       $0  --getid DEVICE          ( return device info )"
  echo "       $0  --getusbip              ( return USBIP exportable devices)"
}


if [ "$1" = "" -o "$1" = "--help" ]; then
  usage
  exit 1
fi

if [ "$output" = "" ]; then
  output="unknow"
fi

if [ "$need_parse" = "1" ]; then
  if [ "$output" != "tmp_file" ]; then
    echo "$output" > $tmp_file
  fi
  num_lines=$(cat $tmp_file | wc -l)
  for i in $(seq 1 $num_lines); do
    line=$(read_line $i)
    echo -n "$line|"
  done
  rm $tmp_file
else
  echo -n $output
fi

exit 0
