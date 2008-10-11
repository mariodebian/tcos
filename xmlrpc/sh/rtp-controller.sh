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
VOLUME=85

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
  MASTER=$(/sbin/soundctl.sh --getlevel Master 2>/dev/null | sed 's/%//g')
  PCM=$(/sbin/soundctl.sh --getlevel PCM 2>/dev/null | sed 's/%//g')
  FRONT=$(/sbin/soundctl.sh --getlevel Front 2>/dev/null | sed 's/%//g')
  MIC=$(/sbin/soundctl.sh --getlevel Mic 2>/dev/null | sed 's/%//g')
  MIXER="tmixer -c 0 "
else
  STANDALONE=1
  STANDALONE_USER=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }' |head -1)
  STANDALONE_USER=$(/usr/lib/tcos/tcos-last --user 2>/dev/null)
  if [ "${STANDALONE_USER}" = "" ]; then echo "error: no standalone user connected"; exit 1; fi
  DBUS_HANDLER="/usr/lib/tcos/tcos-dbus-helper --username=${STANDALONE_USER} "
  DAEMONIZE="/usr/lib/tcos/daemonize.sh"
  MASTER=$(/usr/lib/tcos/soundctl.sh --getlevel Master 2>/dev/null | sed 's/%//g')
  PCM=$(/usr/lib/tcos/soundctl.sh --getlevel PCM 2>/dev/null | sed 's/%//g')
  FRONT=$(/usr/lib/tcos/soundctl.sh --getlevel Front 2>/dev/null | sed 's/%//g')
  MIC=$(/usr/lib/tcos/soundctl.sh --getlevel Mic 2>/dev/null | sed 's/%//g')
  MIXER="/usr/lib/tcos/tmixer -c 0 "
fi

set_volume() {
  [ "$MASTER" = "unknow" ] && MASTER=100
  [ "$PCM" = "unknow" ] && PCM=100
  [ "$FRONT" = "unknow" ] && FRONT=100

  $MIXER sset 'PCM' 'on' 2>/dev/null
  $MIXER sset 'Master' 'on' 2>/dev/null
  $MIXER sset 'Front' 'on' 2>/dev/null
  [ $PCM -lt $VOLUME ] && $MIXER sset 'PCM' $VOLUME 2>/dev/null
  [ $MASTER -lt $VOLUME ] && $MIXER sset 'Master' $VOLUME 2>/dev/null
  [ $FRONT -lt $VOLUME ] && $MIXER sset 'Front' $VOLUME 2>/dev/null
}

set_mic() {
  [ "$MIC" = "unknow" ] && MIC=100

  $MIXER sset 'Mic' 'on' 2>/dev/null
  $MIXER sset 'Mic Boost (+20dB)' 'on' 2>/dev/null
  $MIXER sset 'Mic Boost' 'on' 2>/dev/null
  [ $MIC -lt $VOLUME ] && $MIXER sset 'Mic' $VOLUME 2>/dev/null
}

version=$(pactl --version 2>/dev/null | awk '{print $2}' | awk -F"." '{if ((int($2) >= 9) && (int($3) >= 10)) printf "yes"}')

for arg in $1; do
  case $arg in
     startrtp-recv)
        set_volume
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
        set_volume
        set_mic
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
        set_volume
        set_mic
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
