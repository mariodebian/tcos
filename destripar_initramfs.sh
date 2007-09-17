#!/bin/sh

if [ "$(whoami)" != "root" ]; then
  echo "Please exec as root!!!"
  exit 1
fi

target=/tmp/inittmp

rm -rf $target
mkdir $target
cd $target
cp /tftpboot/tcos/initramfs-2.6.22-2-486 ./initrd.gz
gunzip initrd.gz
cpio -i < initrd
rm -f initrd
mount -o loop -t squashfs /tftpboot/tcos/usr-2.6.22-2-486.squashfs usr/

echo "Exit or Ctrl + d to quit"
bash

#mkdir proc
#mount -t proc none proc/
#chroot . /bin/sh

# exit
echo ""
echo -n "Cleaning mount point..."
umount usr/
#umount proc/
cd ..
rm -rf $target
echo " Done."
