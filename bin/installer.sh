#!/bin/sh

. /conf/tcos.conf
. /conf/tcos-run-functions

# size of vfat partition
hda1_size=100


FDISK=/sbin/fdisk

# translatable strings
MSG_TITLE="TCOS hdd installer"
MSG_WELCOME="Welcome to TCOS hdd installer.\nDo you want to install TCOS in thin client hdd?"
MSG_CANCEL="Cancelled, exiting now..."
MSG_NOHDD="Not hard disk detected. Check if TCOS_DISABLE_IDE is set in tcos.conf."
MSG_HDD_DETECTED="Detected partitions:\n\n"
MSG_DELETE_ALL_DISK="Do you want to delete entire first hard disk?\n\nThis installer will create 2 parts:\n* /dev/hda1 vfat with kernel\n* /dev/hda2 swap (hdd memory)\nRECOMENDED: yes"
MSG_NOPARTIIONS="No partitions detected, please rerun installer.sh and select all disk."

EXIT () {
 # fixme clean umount and do some things here
 exit $1
}


MSG () {
whiptail --title "$MSG_TITLE" "$@" 12 80
}

LIST () {
whiptail --title "$MSG_TITLE" "$@" 20 80
}


make_all_parts () {

# hda hdd size (in MB)
hda_size=$(LANG=C $FDISK -l|grep ^Disk|head -1 | awk '{print $5/1000000}')

# swap (in MB) = Total_RAM (kB) * 2 / 1000
swap_size=$(awk '/MemTotal/ {print int($2*2/1000)}' /proc/meminfo)

# ext3=rest
#hda1_size=$(echo "$hda_size $swap_size" | awk '{print int($1-$2)}')


# clean MBR
echo -n "Cleaning MBR of /dev/hda"
dd if=/dev/zero of=/dev/hda bsize=512 count=1 2>/dev/null >/dev/null
echo " done"

echo -n "Make 2 partitions in /dev/hda, hda1=${hda1_size}M hda2=${swap_size}M"
# call fdisk
$FDISK /dev/hda << EOF 2>/dev/null >/dev/null
o
n
p
1
1
+${hda1_size}M
n
p
2

+${swap_size}M
t
1
b
t
2
82
a
1
w
EOF
echo " done"

# format parts
echo -n "Format hda1 as vfat filesystem (${hda1_size}M)..."
mkfs.vfat /dev/hda1 2>/dev/null >/dev/null
echo " done"

echo -n "Format hda2 as swap (${swap_size}M)..."
mkswap /dev/hda2 2>/dev/null >/dev/null
echo " done"

}


install_kernel () {
# mount hda1
mkdir -p /target
mount /dev/hda1 /target

echo -n "Downloading kernel..."
download_file /tcos/vmlinuz-$(uname -r)  /target/vmlinuz > /dev/null
echo " done."

echo -n "Downloading initramfs..."
download_file /tcos/initramfs-$(uname -r) /target/initrd.gz > /dev/null
download_file /tcos/initramfs-$(uname -r)-nfs /target/initrd.nfs > /dev/null
echo " done."

echo -n "Downloading startup logos and help..."
download_file /tcos/tcos.msg /target/tcos.msg
download_file /tcos/help.msg /target/help.msg
download_file /tcos/logo.lss /target/logo.lss
echo " done."

echo -n "Installing syslinux..."
cat /usr/lib/syslinux/mbr.bin  > /dev/hda
syslinux /dev/hda1

cat << EOF > /target/syslinux.cfg
DEFAULT tcos
TIMEOUT 50
DISPLAY tcos.msg
F0 tcos.msg
F1 help.msg
PROMPT 1

LABEL tcos
        KERNEL vmlinuz
        APPEND ramdisk_size=65536 initrd=initrd.gz root=/dev/ram0 BOOT=tcos quiet splash

LABEL nfs
        KERNEL vmlinuz
        APPEND ramdisk_size=65536 initrd=initrd.nfs root=/dev/ram0 BOOT=tcos quiet splash
EOF
echo " done."

# umount vfat partition
sync
umount /target 
}


format_part () {
  #$1 is partition
  mkfs.ext3 "$1"
}


# update local files if "--update" is passed
if [ "$1" = "--update" ]; then
    echo "Updating installed system..."
    install_kernel
    exit 0
fi


# welcome message
MSG --defaultno --yesno "$MSG_WELCOME" 10 80
if [ $? != 0 ]; then
  MSG --msgbox "$MSG_CANCELLED"
  EXIT 1
fi

# ask for delete all disk
MSG --defaultno --yesno "$MSG_DELETE_ALL_DISK"
if [ $? != 0 ]; then
  TCOS_TYPE="one"
else
  TCOS_TYPE="entire"
fi




if [ "$TCOS_TYPE" = "one" ]; then

   parts=$(cat /proc/partitions | awk '{print $4}'| grep "^[hs]d[a-z][0-9]")
   num_parts=$(cat /proc/partitions | awk '{print $4}'| grep -c "^[hs]d[a-z][0-9]")

   if [ "$num_parts" = "0" ]; then
      MSG --msgbox "$MSG_NOPARTIIONS"
      EXIT 1
   fi

   # show partitions
   parts_types=$($FDISK -l |grep ^/dev | awk '{ if ($2 == "*") {print $1":"$7}; if ($2 != "*") {print $1":"$6" "$7}; }')
   LIST --msgbox "$MSG_HDD_DETECTED${parts_types}"

   parts=""
   for part in $parts_types; do
     device=$(echo $part | awk -F ":" '{print $1}' | awk -F "/" '{print $3}')
     type=$(echo $part | awk -F ":" '{print $2}')
     parts="$parts $device $type"
   done
   #echo $parts
   whiptail --title "$TITLE" --menu "Select part:" 20 80 5 $parts 2> /tmp/tcos_selected_part
   selected_part=$(cat /tmp/tcos_selected_part) && rm /tmp/tcos_selected_part

    # ask for format
    MSG --defaultno --yesno "Do you want to format $selected_part?"
    if [ $? != 0 ]; then
        MSG --msgbox "$MSG_CANCELLED"
    EXIT 1
    else
        format_part /dev/$selected_part
    fi

else

 make_all_parts
 install_kernel
 
fi


