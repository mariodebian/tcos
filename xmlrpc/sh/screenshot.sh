#!/bin/sh

# screenshot.sh shell script to make and screenshot with scrot
#               and save it in /var/www
#  2006-09-09 14:22:40 mariodebian $
#
# This file is part of tcosxmlrpc.
#
# tcosxmlrpc is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# tcosxmlrpc is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with tcosxmlrpc; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.

export DISPLAY=:0


if [ -e /conf/tcos-run-functions ]; then
  _www=/var/www
else
  _www=/var/lib/tcos/standalone/www
  user=$(who | awk '{ if ($2 == ":0") print $1 }')
  if [ "$user" = "root" ]; then echo "error: root not allowed"; exit 1; fi
  export XAUTHORITY=/home/$user/.Xauthority
fi

[ -d $_www ]  && rm -f $_www/*.png


_tmp=/tmp/screenshot
_port=8081
_thumb_size=65

if [ "$1" != "" ]; then
  _thumb_size=$1
fi


rm -rf $_tmp
mkdir $_tmp
cd $_tmp

scrot 'capture.png' -t $_thumb_size

mkdir -p $_www
mv *png $_www

cd $_www
rm -rf $_tmp

_files=$(ls *png)

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
 #echo "<a href=\"$_file\">  <img src=\"$_file\">  </a><br>" >> $_www/index.html
 echo "<a href=\"$_file\">$_file</a><br>" >> $_www/index.html
done

cat << EOF >> $_www/index.html
</body>
</html>
EOF

exit 0
