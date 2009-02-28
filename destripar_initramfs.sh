#!/bin/sh

if [ "$(whoami)" != "root" ]; then
  echo "Please exec as root!!!"
  exit 1
fi

target=/tmp/inittmp

rm -rf $target
mkdir $target
cd $target
cp /var/lib/tcos/tftp/initramfs-2.6.26-1-486 ./initrd.gz
gunzip initrd.gz
cpio -i < initrd
rm -f initrd
unsquashfs /var/lib/tcos/tftp/usr-2.6.26-1-486.squashfs
rsync -az squashfs-root/* usr/
rm -rf squashfs-root
fdupes -r ./ > dupes.txt
ls 


echo "Exit or Ctrl + d to quit"
bash

#mkdir proc
#mount -t proc none proc/
#chroot . /bin/sh

# exit
echo ""
echo -n "Cleaning mount point..."
cd ..
rm -rf $target
echo " Done."
