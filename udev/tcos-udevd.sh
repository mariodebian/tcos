#!/bin/sh
# tcos-udev.sh
# exec by udevd in all block events:
# SUBSYSTEM=="block"

output_file=/tmp/tcos-udevd.log

get_env_var() {
  env_var=$(env |grep ^$1=)
  echo $env_var
}


   id_bus=$(get_env_var "ID_BUS")
   device=$(get_env_var "DEVNAME")
  devpath=$(get_env_var "DEVPATH")
  blockname=$(echo $devpath | awk -F"/" '{print $3}')

if [ $(echo $DEVNAME | grep -c "/dev/loop") != 0 ]; then
   exit 0
fi
if [ $(echo $DEVNAME | grep -c "/dev/ram") != 0 ]; then
   exit 0
fi

   action=$(get_env_var "ACTION")
    label=$(get_env_var "ID_FS_LABEL")
  fs_type=$(get_env_var "ID_FS_TYPE")

if [ "$fs_type" = "ID_FS_TYPE=" ] || [ "$fs_type" = "" ]; then
  fs=$(get_filesystem "${device#DEVNAME=}" --only)
  if [ "$fs" != "auto" ] && [ "$fs" != "" ] && [ "$fs" != "unknow" ] && [ "$fs" != "error, need device" ] ; then
     fs_type="ID_FS_TYPE=$fs"
  fi
fi

   vendor=$(get_env_var "ID_VENDOR")
    model=$(get_env_var "ID_MODEL")

if [ "$vendor" = "ID_VENDOR=" ] || [ "$vendor" = "" ]; then
  vendor="ID_VENDOR="$(cat $(dirname $(find /sys/class/scsi_disk/*/device/  -name "block:$blockname") 2>/dev/null)/vendor 2>/dev/null)
fi

if [ "$model" = "ID_MODEL=" ] || [ "$model" = "" ]; then
  model="ID_MODEL="$(cat $(dirname $(find /sys/class/scsi_disk/*/device/  -name "block:$blockname") 2>/dev/null)/model 2>/dev/null)
fi


echo "$id_bus#$device#$action#$label#$fs_type#$vendor#$model#$devpath" >> $output_file

exit 0
