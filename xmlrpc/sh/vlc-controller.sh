#!/bin/sh
#/*
#* vnc-controller.sh
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
#  VLC controller script for TCOS
#
#

if [ ! -e /dev/dsp ] && [ ! -d /proc/asound ]; then
  echo "error: not soundcard"
  exit 1
fi


export DISPLAY=:0

if [ -e /conf/tcos-run-functions ]; then
  # running in thin client
  STANDALONE=0
  export XAUTHORITY=/root/.Xauthority
  MASTER=$(/sbin/soundctl.sh --getlevel Master 2>/dev/null | sed 's/%//g')
  MASTER_FRONT=$(/sbin/soundctl.sh --getlevel "Master Front" 2>/dev/null | sed 's/%//g')
  PCM=$(/sbin/soundctl.sh --getlevel PCM 2>/dev/null | sed 's/%//g')
  FRONT=$(/sbin/soundctl.sh --getlevel Front 2>/dev/null | sed 's/%//g')
  MIXER="tmixer -c 0 "
else
  MASTER=$(/usr/lib/tcos/soundctl.sh --getlevel Master 2>/dev/null | sed 's/%//g')
  MASTER_FRONT=$(/usr/lib/tcos/soundctl.sh --getlevel "Master Front" 2>/dev/null | sed 's/%//g')
  PCM=$(/usr/lib/tcos/soundctl.sh --getlevel PCM 2>/dev/null | sed 's/%//g')
  FRONT=$(/usr/lib/tcos/soundctl.sh --getlevel Front 2>/dev/null | sed 's/%//g')
  MIXER="/usr/lib/tcos/tmixer -c 0 "
fi

[ "$MASTER" = "unknow" ] && MASTER=100
[ "$MASTER_FRONT" = "unknow" ] && MASTER_FRONT=100
[ "$PCM" = "unknow" ] && PCM=100
[ "$FRONT" = "unknow" ] && FRONT=100

$MIXER sset 'PCM' 'on' >/dev/null 2>&1
$MIXER sset 'Master' 'on' >/dev/null 2>&1
$MIXER sset 'Master Front' 'on' >/dev/null 2>&1
$MIXER sset 'Front' 'on' >/dev/null 2>&1
[ $PCM -lt ${1} ] && $MIXER sset 'PCM' ${1} >/dev/null 2>&1
[ $MASTER -lt ${1} ] && $MIXER sset 'Master' ${1} >/dev/null 2>&1
[ $MASTER_FRONT -lt ${1} ] && $MIXER sset 'Master Front' ${1} >/dev/null 2>&1
[ $FRONT -lt ${1} ] && $MIXER sset 'Front' ${1} >/dev/null 2>&1

