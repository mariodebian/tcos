#!/bin/sh
#/*
#* pactl-controller.sh
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
#  pactl controller script for TCOS
#

MIXER="/usr/lib/tcos/tmixer -c 0 "
VOLUME=85
VOLUME_MIC=75



case $1 in
   start-server)
       MASTER=$(/usr/lib/tcos/soundctl.sh --getlevel Master 2>/dev/null | sed 's/%//g')
       MASTER_FRONT=$(/usr/lib/tcos/soundctl.sh --getlevel "Master Front" 2>/dev/null | sed 's/%//g')
       PCM=$(/usr/lib/tcos/soundctl.sh --getlevel PCM 2>/dev/null | sed 's/%//g')
       FRONT=$(/usr/lib/tcos/soundctl.sh --getlevel Front 2>/dev/null | sed 's/%//g')
       MIC=$(/usr/lib/tcos/soundctl.sh --getlevel Mic 2>/dev/null | sed 's/%//g')
       FRONT_MIC=$(/usr/lib/tcos/soundctl.sh --getlevel "Front Mic" 2>/dev/null | sed 's/%//g')
       REAR_MIC=$(/usr/lib/tcos/soundctl.sh --getlevel "Rear Mic" 2>/dev/null | sed 's/%//g')
       [ "$MASTER" = "unknow" ] && MASTER=$VOLUME
       [ "$MASTER_FRONT" = "unknow" ] && MASTER_FRONT=$VOLUME
       [ "$PCM" = "unknow" ] && PCM=$VOLUME
       [ "$FRONT" = "unknow" ] && FRONT=$VOLUME
       [ "$MIC" = "unknow" ] && MIC=$VOLUME_MIC
       [ "$FRONT_MIC" = "unknow" ] && FRONT_MIC=$VOLUME_MIC
       [ "$REAR_MIC" = "unknow" ] && REAR_MIC=$VOLUME_MIC

       $MIXER sset 'PCM' 'on' >/dev/null 2>&1
       $MIXER sset 'Master' 'on' >/dev/null 2>&1
       $MIXER sset 'Master Front' 'on' >/dev/null 2>&1
       $MIXER sset 'Front' 'on' >/dev/null 2>&1
       [ $PCM -lt $VOLUME ] && $MIXER sset 'PCM' $VOLUME >/dev/null 2>&1
       [ $MASTER -lt $VOLUME ] && $MIXER sset 'Master' $VOLUME >/dev/null 2>&1
       [ $MASTER_FRONT -lt $VOLUME ] && $MIXER sset 'Master Front' $VOLUME >/dev/null 2>&1
       [ $FRONT -lt $VOLUME ] && $MIXER sset 'Front' $VOLUME >/dev/null 2>&1

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
   ;;
   stop-server)
       $MIXER sset 'Mic' 'off' >/dev/null 2>&1
       $MIXER sset 'Front Mic' 'off' >/dev/null 2>&1
       $MIXER sset 'Rear Mic' 'off' >/dev/null 2>&1
       $MIXER sset 'Capture' nocap >/dev/null 2>&1
       $MIXER sset 'Capture,0' nocap >/dev/null 2>&1
       $MIXER sset 'Capture,1' nocap >/dev/null 2>&1
       $MIXER sset 'Capture,2' nocap >/dev/null 2>&1
       $MIXER sset 'Capture,3' nocap >/dev/null 2>&1
   ;;
   start-recv)
       pactl load-module module-rtp-recv sap_address=$2 >/dev/null 2>&1
   ;;
   stop-recv)
       pactl unload-module $2 >/dev/null 2>&1
   ;;
   start-send)
       pactl load-module module-rtp-send format=s16be channels=2 rate=44100 source=@DEFAULT_SOURCE@ loop=0 destination=$2 >/dev/null 2>&1
   ;;
   stop-send)
       pactl unload-module $2 >/dev/null 2>&1
   ;;
   start-chat)
       pactl load-module module-rtp-send format=s16be channels=2 rate=44100 source=@DEFAULT_SOURCE@ loop=0 destination=$2 >/dev/null 2>&1
       pactl load-module module-rtp-recv sap_address=$2 >/dev/null 2>&1
   ;;
   stop-chat)
       pactl unload-module $2 >/dev/null 2>&1
       pactl unload-module $3 >/dev/null 2>&1
   ;;
esac

exit 0
