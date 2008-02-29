#!/bin/sh

#/*
#* screenshot.sh shell script to make and screenshot with scrot
#*               and save it in /var/www
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
  #user=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }')
  user=$(/usr/lib/tcos/tcos-last --user)
  if [ "$user" = "root" ]; then echo -n "error: root not allowed"; exit 1; fi
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

_files=$(ls *jpg)


cat << EOF > $_www/index.html
<html>
<head>
<title>Screenshots</title>
</head>
<body>
<H1>Screenshots of $(hostname),<br>take on $(date)</H1>
<br><br>
EOF
for _file in $_files; do
 echo "<a href=\"$_file\">$_file</a><br>" >> $_www/index.html
done

cat << EOF >> $_www/index.html
</body>
</html>
EOF

echo -n "ok"

exit 0
