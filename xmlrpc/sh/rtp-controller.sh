#!/bin/sh
#
# rtp-controller.sh
# Copyright (C) 2006,2007,2008  vidal_joshur at gva.es
# Copyright (C) 2006-2011  mariodebian at gmail
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

#
#  rtp controller script for TCOS
#

export DISPLAY=:0
VOLUME=85
VOLUME_MIC=75

if [ -e /conf/tcos-run-functions ]; then
  # running in thin client
  STANDALONE=0
  export XAUTHORITY=/root/.Xauthority

  DAEMONIZE="/sbin/daemonize.sh"
  PACTL="/sbin/pactl-controller.sh"
  MASTER=$(/sbin/soundctl.sh --getlevel Master 2>/dev/null | sed 's/%//g')
  MASTER_FRONT=$(/sbin/soundctl.sh --getlevel "Master Front" 2>/dev/null | sed 's/%//g')
  PCM=$(/sbin/soundctl.sh --getlevel PCM 2>/dev/null | sed 's/%//g')
  FRONT=$(/sbin/soundctl.sh --getlevel Front 2>/dev/null | sed 's/%//g')
  MIC=$(/sbin/soundctl.sh --getlevel Mic 2>/dev/null | sed 's/%//g')
  FRONT_MIC=$(/sbin/soundctl.sh --getlevel "Front Mic" 2>/dev/null | sed 's/%//g')
  REAR_MIC=$(/sbin/soundctl.sh --getlevel "Rear Mic" 2>/dev/null | sed 's/%//g')
  MIXER="tmixer -c 0 "
else
  STANDALONE=1
  #STANDALONE_USER=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }' |head -1)
  STANDALONE_USER=$(/usr/lib/tcos/tcos-last --user 2>/dev/null)
  if [ "${STANDALONE_USER}" = "" ]; then echo "error: no standalone user connected"; exit 1; fi
  DBUS_HANDLER="/usr/lib/tcos/tcos-dbus-helper --username=${STANDALONE_USER} "
  DAEMONIZE="/usr/lib/tcos/daemonize.sh"
  PACTL="/usr/lib/tcos/pactl-controller.sh"
  MASTER=$(/usr/lib/tcos/soundctl.sh --getlevel Master 2>/dev/null | sed 's/%//g')
  MASTER_FRONT=$(/usr/lib/tcos/soundctl.sh --getlevel "Master Front" 2>/dev/null | sed 's/%//g')
  PCM=$(/usr/lib/tcos/soundctl.sh --getlevel PCM 2>/dev/null | sed 's/%//g')
  FRONT=$(/usr/lib/tcos/soundctl.sh --getlevel Front 2>/dev/null | sed 's/%//g')
  MIC=$(/usr/lib/tcos/soundctl.sh --getlevel Mic 2>/dev/null | sed 's/%//g')
  FRONT_MIC=$(/usr/lib/tcos/soundctl.sh --getlevel "Front Mic" 2>/dev/null | sed 's/%//g')
  REAR_MIC=$(/usr/lib/tcos/soundctl.sh --getlevel "Rear Mic" 2>/dev/null | sed 's/%//g')
  MIXER="/usr/lib/tcos/tmixer -c 0 "
fi

set_volume() {
  [ "$MASTER" = "unknow" ] && MASTER=$VOLUME
  [ "$MASTER_FRONT" = "unknow" ] && MASTER_FRONT=$VOLUME
  [ "$PCM" = "unknow" ] && PCM=$VOLUME
  [ "$FRONT" = "unknow" ] && FRONT=$VOLUME

  $MIXER sset 'PCM' 'on' >/dev/null 2>&1
  $MIXER sset 'Master' 'on' >/dev/null 2>&1
  $MIXER sset 'Master Front' 'on' >/dev/null 2>&1
  $MIXER sset 'Front' 'on' >/dev/null 2>&1
  [ $PCM -lt $VOLUME ] && $MIXER sset 'PCM' $VOLUME >/dev/null 2>&1
  [ $MASTER -lt $VOLUME ] && $MIXER sset 'Master' $VOLUME >/dev/null 2>&1
  [ $MASTER_FRONT -lt $VOLUME ] && $MIXER sset 'Master Front' $VOLUME >/dev/null 2>&1
  [ $FRONT -lt $VOLUME ] && $MIXER sset 'Front' $VOLUME >/dev/null 2>&1
}

set_mic() {
  [ "$MIC" = "unknow" ] && MIC=$VOLUME_MIC
  [ "$FRONT_MIC" = "unknow" ] && FRONT_MIC=$VOLUME_MIC
  [ "$REAR_MIC" = "unknow" ] && REAR_MIC=$VOLUME_MIC

  $MIXER sset 'Mic' 'on' >/dev/null 2>&1
  $MIXER sset 'Front Mic' 'on' >/dev/null 2>&1
  $MIXER sset 'Rear Mic' 'on' >/dev/null 2>&1
  $MIXER sset 'Mic Boost (+20dB)' 'off' >/dev/null 2>&1
  $MIXER sset 'Mic Boost' 'off' >/dev/null 2>&1
  $MIXER sset 'Front Mic Boost' 'off' >/dev/null 2>&1
  $MIXER sset 'Rear Mic Boost' 'off' >/dev/null 2>&1
  $MIXER sset 'Mic Boost (+20dB)' 0% >/dev/null 2>&1
  $MIXER sset 'Mic Boost' 0% >/dev/null 2>&1
  $MIXER sset 'Front Mic Boost' 0% >/dev/null 2>&1
  $MIXER sset 'Rear Mic Boost' 0% >/dev/null 2>&1
  $MIXER sset 'Mic' $VOLUME_MIC >/dev/null 2>&1
  $MIXER sset 'Front Mic' $VOLUME_MIC >/dev/null 2>&1
  $MIXER sset 'Rear Mic' $VOLUME_MIC >/dev/null 2>&1
  $MIXER sset 'Capture' cap >/dev/null 2>&1
  $MIXER sset 'Capture,0' cap >/dev/null 2>&1
  $MIXER sset 'Capture,1' cap >/dev/null 2>&1
  $MIXER sset 'Capture,2' cap >/dev/null 2>&1
  $MIXER sset 'Capture,3' cap >/dev/null 2>&1
  $MIXER sset 'Capture' 90% >/dev/null 2>&1
  $MIXER sset 'Capture,0' 90% >/dev/null 2>&1
  $MIXER sset 'Capture,1' 90% >/dev/null 2>&1
  $MIXER sset 'Capture,2' 90% >/dev/null 2>&1
  $MIXER sset 'Capture,3' 90% >/dev/null 2>&1
}

set_mute_mic() {
  $MIXER sset 'Mic' 'off' >/dev/null 2>&1
  $MIXER sset 'Front Mic' 'off' >/dev/null 2>&1
  $MIXER sset 'Rear Mic' 'off' >/dev/null 2>&1
  $MIXER sset 'Capture' nocap >/dev/null 2>&1
  $MIXER sset 'Capture,0' nocap >/dev/null 2>&1
  $MIXER sset 'Capture,1' nocap >/dev/null 2>&1
  $MIXER sset 'Capture,2' nocap >/dev/null 2>&1
  $MIXER sset 'Capture,3' nocap >/dev/null 2>&1
}


version=$(pactl --version 2>/dev/null | awk '{print $2}' | awk -F"." '{if ((int($2) >= 9) && (int($3) >= 10)) printf "yes"}')

for arg in $1; do
  case $arg in
     startrtp-recv)
        set_volume
        set_mute_mic
        if [ ! -z $version ]; then
            $DAEMONIZE "$PACTL" "start-recv $2"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting pulse recv module"; fi
        else
            echo "error: pulseaudio version not supported"
        fi
     ;;
     stoprtp-recv)
        if [ ! -z $version ]; then
           index=$(pactl list | grep -B1 "module-rtp-recv" | head -1 | awk '{printf $3}' | sed 's/#//g')
           $DAEMONIZE "$PACTL" "stop-recv $index"
            if [ $? = 0 ]; then echo "ok"; else echo "error: stopping pulse recv module"; fi
        else
            echo "error: pulseaudio version not supported"
        fi
     ;;
     startrtp-send)
        set_volume
        set_mic
        if [ ! -z $version ]; then
            $DAEMONIZE "$PACTL" "start-send $2"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting pulse send module"; fi
        else
            echo "error: starting pulse send module, incorrect pulseaudio version"
        fi
     ;;
     stoprtp-send)
        set_mute_mic
        if [ ! -z $version ]; then
           index=$(pactl list | grep -B1 "module-rtp-send" | head -1 | awk '{printf $3}' | sed 's/#//g')
           $DAEMONIZE "$PACTL" "stop-send $index"
            if [ $? = 0 ]; then echo "ok"; else echo "error: stopping pulse send module"; fi
        else
            echo "error: stopping pulse send module, incorrect pulseaudio version"
        fi
     ;;
     startrtp-chat)
        set_volume
        set_mic
        if [ ! -z $version ]; then
            $DAEMONIZE "$PACTL" "start-chat $2"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting pulse chat mode"; fi
        else
            echo "error: starting pulse chat mode, incorrect pulseaudio version"
        fi
     ;;
     stoprtp-chat)
        set_mute_mic
        if [ ! -z $version ]; then
           index_send=$(pactl list | grep -B1 "module-rtp-send" | head -1 | awk '{printf $3}' | sed 's/#//g')
           index_recv=$(pactl list | grep -B1 "module-rtp-recv" | head -1 | awk '{printf $3}' | sed 's/#//g')
           $DAEMONIZE "$PACTL" "stop-chat $index_send $index_recv"
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
