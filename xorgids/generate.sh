#!/bin/sh

OUTPUT=xorg.pciids
cat /dev/null > $OUTPUT


for i in /usr/share/xserver-xorg/pci/*ids openchrome.ids vboxvideo.ids; do

  driver=$(basename $i .ids)
  echo " * parsing driver $driver" >&2
  for _line in $(cat $i); do
    line=$(echo $_line | awk '{print substr($1,0,4)":"substr($1,5,8)}' | tr '[a-z]' '[A-Z]')
    echo "$driver $line" >> $OUTPUT
  done

done
