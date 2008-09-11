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

else
  STANDALONE=1
  STANDALONE_USER=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }' |head -1)
  STANDALONE_USER=$(/usr/lib/tcos/tcos-last --user 2>/dev/null)
  if [ "${STANDALONE_USER}" = "" ]; then echo "error: no standalone user connected"; exit 1; fi
  DBUS_HANDLER="/usr/lib/tcos/tcos-dbus-helper --username=${STANDALONE_USER} "
fi



for arg in $1; do
  case $arg in
     startrtp)
        mkfifo /tmp/audiofifo 2>/dev/null
        if [ $STANDALONE = 0 ]; then
             cat << EOF > /etc/asound.conf
pcm.!default {
    type pulse
}
ctl.!default {
    type pulse
}
EOF
             /sbin/daemonize.sh "rtpdump" "-F payload -o /tmp/audiofifo $2/1234"
             /sbin/daemonize.sh "play" "-t raw -r 44100 -s -2 -c 2 -B /tmp/audiofifo"
             if [ $? = 0 ]; then echo "ok"; else echo "error: starting rtp play"; fi
        else
             /usr/lib/tcos/daemonize.sh "rtpdump" "-F payload -o /tmp/audiofifo $2/1234"
             $DBUS_HANDLER --auth=$3 --type=exec --text="play -t raw -r 44100 -s -2 -c 2 -B /tmp/audiofifo"
        fi
     ;;
     stoprtp)
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
     ;;
     *)
       echo "error"
     ;;
  esac

done
