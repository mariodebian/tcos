#!/bin/sh

if [ "$(whoami)" != "root" ]; then
  echo "Please exec as root!!!"
  exit 1
fi

target=/tmp/inittmp

rm -rf $target
mkdir $target
cd $target
cp /tftpboot/tcos/initramfs ./initrd.gz
gunzip initrd.gz
cpio -i < initrd
rm -f initrd
cp /tftpboot/tcos/usr.squashfs .
mount -o loop -t squashfs usr.squashfs usr/
mkdir proc
mount -t proc none proc/
chroot . /bin/sh

# exit
echo ""
echo -n "Cleaning mount point..."
umount usr/
umount proc/
cd ..
rm -rf $target
echo " Done."
