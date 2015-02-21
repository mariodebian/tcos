#!/bin/sh
#
# screenshot.sh shell script to make and screenshot with scrot
#               and save it in /var/www
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


export DISPLAY=:0

beepoff=""
beepon=""

if [ -e /conf/tcos-run-functions ]; then
  _www=/var/www
  export XAUTHORITY=/root/.Xauthority
else
  _www=/var/lib/tcos/standalone/www
  export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/lib/tcos
  export XAUTHORITY=$(get_xauth)

  beepoff="xset b off"
  beepon="xset b on"
fi


_thumb_size=65

if [ "$1" != "" ]; then
  _thumb_size=$1
fi


mkdir -p $_www
rm -rf $_www/*
cd $_www
$($beepoff)
scrot 'capture.jpg' -t $_thumb_size
$($beepon)

exit 0

