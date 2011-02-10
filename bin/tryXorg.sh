#!/bin/sh
# Copyright (C) 2006,2007,2008  mariodebian at gmail
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

. /conf/tcos.conf

# try to launch Xorg or return error

MAX_XORG_RETRY=3
XORG_FORCE_AUTO=0

export XAUTHORITY=/root/.Xauthority
export HOME=/root
export DISPLAY=:0


run_scripts() {
    [ ! -d $1 ] && return
    for src in $1/*; do
      [ -f $src ] && . $src
    done
}


for x in "$@"; do
        case $x in
        --server=*)
                SERVER=${x#--server=}
                ;;
        --fontserver=*)
                FONT_SERVER=${x#--fontserver=}
                ;;
        --xorgopts=*)
                TCOS_XORG_OPTS=${x#--xorgopts=}
                ;;
        esac
done

counter=1
while [ $counter -le $MAX_XORG_RETRY ]; do
 run_scripts /etc/X11/PreRun
 # start Xorg at vt7 to avoid getty keyboard conflicts
 Xorg vt7 ${TCOS_XORG_OPTS} > /dev/null 2>&1
 counter=$((counter+1))
 [ -f /var/log/Xorg.0.log ] && mv /var/log/Xorg.0.log /var/log/Xorg.log.fail.${counter}
done

# try to autoconfigure Xorg
new_xorg=$(Xorg -configure 2>&1|awk '/Your xorg.conf/ {print $5}')

if [ "${FONT_SERVER}" = "" ]; then
  FONT_SERVER=${SERVER}
fi

if [ "TCOS_ENABLE_FONT_SERVER" = "1" ]; then
# add font server
sed "/modules/i\
\	FontPath     \"tcp/${FONT_SERVER}:7100\"\
\	FontPath     \"tcp/${FONT_SERVER}:7101\"\
" $new_xorg > /etc/X11/xorg.conf.auto
fi

# disable HAL
sed "/ServerLayout/a\
Option \"AutoAddDevices\" \"off\" \
" /etc/X11/xorg.conf.auto > /etc/X11/xorg.conf.auto.new
mv /etc/X11/xorg.conf.auto.new /etc/X11/xorg.conf.auto


# change keyboard map
sed -i "/kbd/i\
\	Option     \"XkbRules\"     \"xorg\"\
" /etc/X11/xorg.conf.auto
sed -i "/kbd/i\
\	Option     \"XkbModel\"     \"${TCOS_XORG_XKBMODEL}\"\
" /etc/X11/xorg.conf.auto
sed -i "/kbd/i\
\	Option     \"XkbLayout\"     \"${TCOS_XORG_XKB}\"\
" /etc/X11/xorg.conf.auto

XORG_FORCE_AUTO=1

run_scripts /etc/X11/PreRun
# start Xorg at vt7 to avoid getty keyboard conflicts
Xorg vt7 -config /etc/X11/xorg.conf.auto ${TCOS_XORG_OPTS}  > /dev/null 2>&1

cat /dev/null > /dev/tty1
chvt 1
cat << EOF > /dev/tty1


Error launching Xorg, check /var/log/Xorg.0.log for errors


   These are the lasts lines of Xorg.0.log
EOF

tail /var/log/Xorg.0.log > /dev/tty1
echo "---------------------------------------------------" > /dev/tty1
grep EE /var/log/Xorg.0.log > /dev/tty1

exit 1
