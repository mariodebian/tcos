#!/bin/sh

#/*
#* screenshot64.sh shell script to make and screenshot with scrot
#*               and save it in /var/www in base64 format
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


export DISPLAY=:0


if [ -e /conf/tcos-run-functions ]; then
  _www=/var/www
  export XAUTHORITY=/root/.Xauthority
else
  _www=/var/lib/tcos/standalone/www
  user=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }')
  if [ "$user" = "root" ]; then echo "error: root not allowed"; exit 1; fi
  export XAUTHORITY=/home/$user/.Xauthority
  export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/lib/tcos
fi



_thumb_size=65

if [ "$1" != "" ]; then
  _thumb_size=$1
fi

mkdir -p $_www
rm -rf $_www/*
cd $_www

scrot 'capture.jpg' -t $_thumb_size
# put base64 text in one line (w=0)
base64 -w 0 < capture-thumb.jpg > capture-thumb.b64 2>/dev/null || (echo -n "error in base64" ; exit 0)

echo -n "ok"

exit 0
