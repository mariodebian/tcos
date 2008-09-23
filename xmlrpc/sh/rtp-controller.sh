#!/bin/sh
#/*
#* rtp-controller.sh
#* Copyright (C) 2006,2007,2008  mariodebian at gmail
#* Copyright (C) 2006,2007,2008  vidal_joshur at gva.es
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

#
#  rtp controller script for TCOS
#

export DISPLAY=:0

if [ -e /conf/tcos-run-functions ]; then
  # running in thin client
  STANDALONE=0
  export XAUTHORITY=/root/.Xauthority
  if [ -d /usr/lib/sox ]  && [ ! -f /tmp/sox.libs ]; then
    # update sox libs
    for lib in $(find /usr/lib/sox/ -type f); do
      ln -s $lib $(echo $lib | awk -F"\.so" '{print $1".so"}')    >> /tmp/sox.libs 2>&1
      ln -s $lib $(echo $lib | awk -F"\.so" '{print $1".so.0"}')  >> /tmp/sox.libs 2>&1
    done
  fi
  cat << EOF > /etc/asound.conf
pcm.!default {
    type pulse
}
ctl.!default {
    type pulse
}
EOF
  DAEMONIZE="/sbin/daemonize.sh"
else
  STANDALONE=1
  STANDALONE_USER=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }' |head -1)
  STANDALONE_USER=$(/usr/lib/tcos/tcos-last --user 2>/dev/null)
  if [ "${STANDALONE_USER}" = "" ]; then echo "error: no standalone user connected"; exit 1; fi
  DBUS_HANDLER="/usr/lib/tcos/tcos-dbus-helper --username=${STANDALONE_USER} "
  DAEMONIZE="/usr/lib/tcos/daemonize.sh"
fi

version=$(pactl --version | awk '{print $2}' | awk -F"." '{if (($2 >= 9) && ($3 >= 10)) printf "yes"}')

for arg in $1; do
  case $arg in
     startrtp-recv)
        if [ ! -z $version ]; then
            $DAEMONIZE "pactl" "load-module module-rtp-recv sap_address=$2"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting pulse recv module"; fi
        else
            mkfifo /tmp/audiofifo 2>/dev/null
            if [ $STANDALONE = 0 ]; then
               $DAEMONIZE "rtpdump" "-F payload -o /tmp/audiofifo $2/1234"
               $DAEMONIZE "play" "-t raw -r 11025 -s -2 -c 2 -B /tmp/audiofifo"
               if [ $? = 0 ]; then echo "ok"; else echo "error: starting rtp play"; fi
            else
               $DAEMONIZE "rtpdump" "-F payload -o /tmp/audiofifo $2/1234"
               $DBUS_HANDLER --auth=$3 --type=exec --text="play -t raw -r 11025 -s -2 -c 2 -B /tmp/audiofifo"
            fi
        fi
     ;;
     stoprtp-recv)
        if [ ! -z $version ]; then
           index=$(pactl list | grep -B1 "module-rtp-recv" | head -1 | awk '{printf $3}' | sed 's/#//g')
           $DAEMONIZE "pactl" "unload-module $index"
            if [ $? = 0 ]; then echo "ok"; else echo "error: stopping pulse recv module"; fi
        else
           if [ $STANDALONE = 0 ]; then
              killall -SIGKILL play 2>/dev/null
              killall -SIGKILL rtpdump 2>/dev/null
              rm -f /tmp/audiofifo /etc/asound.conf
              if [ $? = 0 ]; then echo "ok"; else echo "error: stopping rtp play"; fi
           else
              $DBUS_HANDLER --auth=$2 --type=exec --text="killall -s KILL play" 2>/dev/null
              killall -s KILL rtpdump 2>/dev/null
              rm -f /tmp/audiofifo
              if [ $? = 0 ]; then echo "ok"; else echo "error: stopping rtp play"; fi
           fi
        fi
     ;;
     startrtp-send)
        if [ ! -z $version ]; then
            $DAEMONIZE "pactl" "load-module module-rtp-send destination=$2 rate=11025 channels=2 port=1234"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting pulse send module"; fi
        else
            echo "error: starting pulse send module, incorrect pulseaudio version"
        fi
     ;;
     stoprtp-send)
        if [ ! -z $version ]; then
           index=$(pactl list | grep -B1 "module-rtp-send" | head -1 | awk '{printf $3}' | sed 's/#//g')
           $DAEMONIZE "pactl" "unload-module $index"
            if [ $? = 0 ]; then echo "ok"; else echo "error: stopping pulse send module"; fi
        else
            echo "error: stopping pulse send module, incorrect pulseaudio version"
        fi
     ;;
     startrtp-chat)
        if [ ! -z $version ]; then
            $DAEMONIZE "pactl" "load-module module-rtp-send destination=$2 rate=11025 channels=2 port=1234"
            $DAEMONIZE "pactl" "load-module module-rtp-recv sap_address=$2"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting pulse chat mode"; fi
        else
            echo "error: starting pulse chat mode, incorrect pulseaudio version"
        fi
     ;;
     stoprtp-chat)
        if [ ! -z $version ]; then
           index_send=$(pactl list | grep -B1 "module-rtp-send" | head -1 | awk '{printf $3}' | sed 's/#//g')
           index_recv=$(pactl list | grep -B1 "module-rtp-recv" | head -1 | awk '{printf $3}' | sed 's/#//g')
           $DAEMONIZE "pactl" "unload-module $index_send"
           $DAEMONIZE "pactl" "unload-module $index_recv"
            if [ $? = 0 ]; then echo "ok"; else echo "error: stopping pulse chat mode"; fi
        else
            echo "error: stopping pulse chat mode, incorrect pulseaudio version"
        fi
     ;;
     *)
       echo "error"
     ;;
  esac

done
