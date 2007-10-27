#!/bin/sh

cmd=$1

if [ ! -e $cmd ]; then
  cmd=$(which $1)
fi

args=$(echo $@ | sed "s/$1//g")

/sbin/start-stop-daemon --quiet --background --start --exec $cmd -- $args
