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

if [ -e /dev/dsp ] && [ ! -d /proc/asound ]; then
  TCOS_OSS=1
  MIXER="aumix"
else
  TCOS_OSS=
  MIXER="amixer -c 0"
fi

export DISPLAY=:0

if [ -e /conf/tcos-run-functions ]; then
  # running in thin client
  STANDALONE=0
  export XAUTHORITY=/root/.Xauthority
fi

if [ $TCOS_OSS ]; then
  $MIXER -w "${1}"
  $MIXER -v "${1}"
else
  $MIXER set 'PCM' ${1}% unmute >/dev/null 2>&1
  $MIXER set 'Master' ${1}% unmute >/dev/null 2>&1
  $MIXER set 'Master Digital' ${1}% unmute >/dev/null 2>&1
fi