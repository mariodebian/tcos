#!/bin/sh

cmd=$1

if [ ! -e $cmd ]; then
  cmd=$(which $1)
fi

args=$(echo $@ | sed "s|$1||g")

case $1 in
  poweroff)
    sleep $2
    /sbin/start-stop-daemon --quiet --background --start --exec $cmd -- 
    ;;
  reboot)
    sleep $2
    /sbin/start-stop-daemon --quiet --background --start --exec $cmd -- 
    ;;
  *)
    /sbin/start-stop-daemon --quiet --background --start --exec $cmd -- $args
    ;;
esac
