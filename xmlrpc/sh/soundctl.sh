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

CMD="amixer -c 0 "

output=""
need_parse="0"
unmute_level="80"

if [ -e /dev/dsp ] && [ ! -d /proc/asound ]; then
  TCOS_OSS=1
  MIXER="aumix"
else
  TCOS_OSS=
  MIXER="amixer -c 0 "
  [ $? -eq 1 ] && MIXER="amixer -c 1 "
fi

# for debug force OSS
#TCOS_OSS=1
#MIXER="aumix"
#####################

get_controls() {
  if [ $TCOS_OSS ]; then
    $MIXER -q | awk '{print $1}'
  else
    $MIXER scontrols | awk -F " control " '{print $2}'| awk -F "," '{print $1}' | sed s/"'"//g
  fi
}

get_level() {
  if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
  fi
  
  if [ $TCOS_OSS ]; then
    $MIXER -q | grep $1 | head -1 | awk '{print $3}' | sed s/","//g
  else
    var=$($MIXER  sget "$1" | grep "^  Front"| head -1 | awk '{print $5}'| sed s/"\["//g| sed s/"\]"//g)
    [ -z $var ] && var=$($MIXER  sget "$1" | grep "^  Mono"| head -1 | awk '{print $4}'| sed s/"\["//g| sed s/"\]"//g)
    echo $var
  fi
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
 
 if [ $TCOS_OSS ]; then
    if [ "$1" = "vol" ]; then
      $MIXER -v "$2" ; get_level "$1"
    elif [ "$1" = "pcm" ]; then
      $MIXER -w "$2" ; get_level "$1"
    elif [ "$1" = "line" ]; then
      $MIXER -l "$2" ; get_level "$1"
    elif [ "$1" = "mic" ]; then
      $MIXER -m "$2" ; get_level "$1"
    elif [ "$1" = "cd" ]; then
      $MIXER -c "$2" ; get_level "$1"
    else
      echo "unknow OSS mixer channel"
    fi
 else
    var=$($MIXER  set "$1" "$2" | grep "^  Front"| head -1 | awk '{print $5}'| sed s/"\["//g| sed s/"\]"//g)
    [ -z $var ] && var=$($MIXER  set "$1" "$2" | grep "^  Mono"| head -1 | awk '{print $4}'| sed s/"\["//g| sed s/"\]"//g)
    echo $var
 fi
}

get_mute() {
 if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
 fi
 if [ $TCOS_OSS ]; then
    if [ "$(get_level $1)" = "0" ]; then
      echo "off"
    else
      echo "on"
    fi
  else
    var=$($MIXER  sget $1| grep "^  Front"| head -1 | awk '{print $6}'| sed s/"\["//g| sed s/"\]"//g)
    if [ -z $var ];then
      var=$($MIXER  sget $1| grep "^  Mono"| head -1 | awk '{print $6}'| sed s/"\["//g| sed s/"\]"//g)
    elif [ $var != "on" -a $var != "off" -a $var != "" ];then
      var=$($MIXER  sget $1| grep "^  Front"| head -1 | awk '{print $7}'| sed s/"\["//g| sed s/"\]"//g)
    fi
    echo $var
  fi
}

set_mute() {
 if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
 fi
 if [ $TCOS_OSS ]; then
    set_level "$1" "0"
  else
    var=$($MIXER  set $1 mute| grep "^  Front"| head -1 | awk '{print $6}'| sed s/"\["//g| sed s/"\]"//g)
    if [ -z $var ];then
      var=$($MIXER  set $1 mute| grep "^  Mono"| head -1 | awk '{print $6}'| sed s/"\["//g| sed s/"\]"//g)
    elif [ $var != "on" -a $var != "off" -a $var != "" ];then
      var=$($MIXER  set $1 mute| grep "^  Front"| head -1 | awk '{print $7}'| sed s/"\["//g| sed s/"\]"//g)
    fi
    echo $var
  fi
}

set_unmute() {
 if [ "$1" = "" ]; then
   echo "soundctl error, need a control to retrieve data"
   exit 1
 fi
 if [ $TCOS_OSS ]; then
    set_level "$1" "$unmute_level"
  else
    var=$($MIXER  set $1 unmute| grep "^  Front"| head -1 | awk '{print $6}'| sed s/"\["//g| sed s/"\]"//g)
    if [ -z $var ];then
      var=$($MIXER  set $1 unmute| grep "^  Mono"| head -1 | awk '{print $6}'| sed s/"\["//g| sed s/"\]"//g)
    elif [ $var != "on" -a $var != "off" -a $var != "" ];then
      var=$($MIXER  set $1 unmute| grep "^  Front"| head -1 | awk '{print $7}'| sed s/"\["//g| sed s/"\]"//g)
    fi
    echo $var
  fi
}

read_line() {
head -$1 /tmp/soundctl | tail -1
}

get_serverinfo() {
  if [ "$(pidof pulseaudio)" = "" ]; then
    echo "error: pulseaudio not running"
    exit 1
  fi
  pactl -s 127.0.0.1 stat > /tmp/soundinfo
  output=$(cat /tmp/soundinfo)
  rm /tmp/soundinfo
  need_parse=1
}

usage() {
  echo "Usage:"
  echo "       $0  --help                  ( this help text )"
  echo "       $0  --showcontrols          ( return all mixer channels )"
  echo "       $0  --getlevel CHANNEL      ( return CHANNEL level xx% xx% left and right )"
  echo "       $0  --setlevel CHANNEL xx%  ( change and return CHANNEL level xx% xx% left and right )"
  echo "       $0  --getmute CHANNEL       ( return off if mute or on if unmute CHANNEL )"
  echo "       $0  --setmute CHANNEL       ( mute CHANNEL and return off if succesfull )"
  echo "       $0  --setunmute CHANNEL     ( unmute CHANNEL and return on if succesfull )"
  echo "       $0  --getserverinfo         ( show stats of PulseAudio server with pactl)"
}



for x in $1; do
	case $x in
	--showcontrols)
		output=$(get_controls); need_parse="1"
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

if [ "$need_parse" = "1" ]; then
  echo "$output" > /tmp/soundctl
  num_lines=$(cat /tmp/soundctl | wc -l)
  for i in $(seq 1 $num_lines); do
    line=$(read_line $i)
    echo -n "$line|"
  done
  rm /tmp/soundctl
else
  echo -n $output
fi

exit 0
