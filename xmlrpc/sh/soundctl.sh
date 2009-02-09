#!/bin/sh
# TCOS Sound volume control
#/*
#* soundctl.sh part of tcosxmlrpc
#*   => get and configure sound channels
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


if [ ! -e /dev/dsp ] && [ ! -d /proc/asound ]; then
  echo "error: not soundcard"
  exit 1
fi



output=""
tmpfile=/tmp/soundinfo-$$

read_line() {
  head -$1 $tmpfile | tail -1
}

#####################
#export TMIXER_FORCE=oss
#export TMIXER_OSSMUTELEVEL=85
if [ -x /usr/lib/tcos/tmixer ]; then
  MIXER="/usr/lib/tcos/tmixer -c 0 "
else
  MIXER="tmixer -c 0 "
fi


get_controls() {
  $MIXER scontrols
}



get_contents() {
  $MIXER scontents > $tmpfile
  num_lines=$(cat $tmpfile | wc -l)
  for i in $(seq 1 $num_lines); do
    line=$(read_line $i)
    echo -n "$line#"
  done
  rm $tmpfile
}

get_level() {
  if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
  fi
  $MIXER sget "$1" | awk -F"," '{print $3}'
}

set_level() {
 if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
 fi
 if [ "$2" = "" ]; then
   echo "soundctl error, need a xxx% level or 1-31 int"
   exit 1
 fi
 $MIXER sset "$1" "$2"
}

get_mute() {
 if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
 fi
 $MIXER sget "$1" | awk -F"," '{print $4}'
}

set_mute() {
 if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
 fi
 $MIXER sset "$1" "off"
}

set_unmute() {
 if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
 fi
 $MIXER sset "$1" "on"
}




get_serverinfo() {
  if [ "$(pidof pulseaudio | sed '/^$/d')" = "" ]; then
    echo "error: pulseaudio not running"
    exit 1
  fi
  #pactl -s 127.0.0.1 stat > $tmpfile
  pactl stat > $tmpfile
  num_lines=$(cat $tmpfile | wc -l)
  for i in $(seq 1 $num_lines); do
    line=$(read_line $i)
    echo -n "$line|"
  done
  rm $tmpfile
  exit 0
}

usage() {
  echo "Usage:"
  echo "       $0  --help                  ( this help text )"
  echo "       $0  --showcontrols          ( return all mixer channels )"
  echo "       $0  --showcontents          ( return all mixer channels with data )"
  echo "       $0  --getlevel CHANNEL      ( return CHANNEL level xx% xx% left and right )"
  echo "       $0  --setlevel CHANNEL xx%  ( change and return CHANNEL level xx% xx% left and right )"
  echo "       $0  --getmute CHANNEL       ( return off if mute or on if unmute CHANNEL )"
  echo "       $0  --setmute CHANNEL       ( mute CHANNEL and return off if succesfull )"
  echo "       $0  --setunmute CHANNEL     ( unmute CHANNEL and return on if succesfull )"
  echo "       $0  --getserverinfo         ( show stats of PulseAudio server with pactl)"
  echo "       $0  --restartpulse          ( restart PulseAudio daemon)"
}



for x in $1; do
	case $x in
	--showcontrols)
		output=$(get_controls); 
		;;
	--showcontents)
		output=$(get_contents); 
		;;
    --getlevel)
		output=$(get_level "$2")
		;;
    --setlevel)
		output=$(set_level "$2" "$3")
		;;
    --getmute)
		output=$(get_mute "$2")
		;;
    --setmute)
		output=$(set_mute "$2")
		;;
    --setunmute)
		output=$(set_unmute "$2")
		;;
    --getserverinfo)
		get_serverinfo
		;;
    --restartpulse)
		killall -9 pulseaudio; /bin/daemonize.sh /sbin/startpulseaudio
		;;
    --help)
		usage
		exit 1
		;;
    esac
done




if [ "$1" = "" ]; then
  usage
  exit 1
fi

if [ "$output" = "" ]; then
  output="unknow"
fi

echo -n $output

exit 0
