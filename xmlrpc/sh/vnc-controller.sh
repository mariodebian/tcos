#!/bin/sh
#
# vnc-controller.sh
# Copyright (C) 2006-2011  mariodebian at gmail
# Copyright (C) 2006,2007,2008  vidal_joshur at gva.es
#
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
#  VNC controller script for TCOS
#
# examples:
#   vnc-controller.sh startserver "authfile"
#   vnc-controller.sh startserver "authfile" "user:pass"
#
#   vnc-controller.sh startscale "authfile" "__scaleX__x__scaleY__"
#   vnc-controller.sh startscale "authfile" "__scaleX__x__scaleY__" "user:pass"
#
#   vnc-controller.sh stopserver
#   vnc-controller.sh stopserver "user:pass"
#
#   vnc-controller.sh startclient "ip" "authfile"
#   vnc-controller.sh startclient "ip" "authfile" "user:pass"
#
#   vnc-controller.sh stopclient
#
#   vnc-controller.sh genpass "pass" "file"
#   vnc-controller.sh genpass "pass" "file" "user:pass"

export DISPLAY=:0

if [ -e /conf/tcos-run-functions ]; then
  # running in thin client
  STANDALONE=0
  export XAUTHORITY=/root/.Xauthority
else
  STANDALONE=1
  #STANDALONE_USER=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }' |head -1)
  STANDALONE_USER=$(/usr/lib/tcos/tcos-last --user 2>/dev/null)
  if [ "${STANDALONE_USER}" = "" ]; then echo "error: no standalone user connected"; exit 1; fi
  DBUS_HANDLER="/usr/lib/tcos/tcos-dbus-helper --username=${STANDALONE_USER} "
fi

REOPEN=""

if x11vnc --help| grep -q "^\-reopen"; then
  REOPEN=" -reopen "
fi

for arg in $1; do
  case $arg in
     startserver)
        cmd=" -shared -noshm -forever $REOPEN -rfbauth $2"
        if [ $STANDALONE = 0 ]; then
             killall -SIGKILL x11vnc
            /sbin/daemonize.sh "x11vnc" "$cmd"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting vnc server"; fi
        else
            $DBUS_HANDLER --auth=$3 --type=exec --text="killall -s KILL x11vnc" 2>/dev/null
            # this returns ok if can send dbus msg, no need to parse
            $DBUS_HANDLER --auth=$3 --type=exec --text="x11vnc $cmd" 2>/dev/null
        fi
     ;;
     startscale)
        cmd=" -shared -noshm -forever $REOPEN -rfbauth $2 -scale $3"
        if [ $STANDALONE = 0 ]; then
             killall -SIGKILL x11vnc
            /sbin/daemonize.sh "x11vnc" "$cmd"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting vnc server"; fi
        else
            $DBUS_HANDLER --auth=$4 --type=exec --text="killall -s KILL x11vnc" 2>/dev/null
            # this returns ok if can send dbus msg, no need to parse
            $DBUS_HANDLER --auth=$4 --type=exec --text="x11vnc $cmd" 2>/dev/null
        fi
     ;;
     stopserver)
        if [ $STANDALONE = 0 ]; then
           killall -SIGKILL x11vnc
           # killall return != 0 if no proccess killed
           #if [ $? = 0 ]; then echo "ok"; else echo "error: killing vnc server"; fi
           echo "ok"
        else
           $DBUS_HANDLER --auth=$2 --type=exec --text="killall -s KILL x11vnc"
        fi
        
     ;;
     startclient)
        # exit if pass file not exists
        if [ ! -f "$3" ]; then echo "error: no passwd file"; exit 1; fi
        
        # check vncviewer version
        VNC_NEW_VERSION=$(vncviewer --version 2>&1| grep built |grep -c "4.1")
        if [ $STANDALONE = 0 ]; then
            if [ $VNC_NEW_VERSION = 1 ]; then
               /sbin/daemonize.sh "vncviewer" "$2 -ViewOnly -FullScreen -UseLocalCursor=0 -PasswordFile $3"
               if [ $? = 0 ]; then echo "ok"; else echo "error: starting vncviewer"; fi
            else
               /sbin/daemonize.sh "vncviewer" "$2 -viewonly -fullscreen -passwd $3"
               if [ $? = 0 ]; then echo "ok"; else echo "error: starting vncviewer"; fi
            fi
        else
            if [ $VNC_NEW_VERSION = 1 ]; then
               $DBUS_HANDLER --auth=$4 --type=exec --text="vncviewer $2 -ViewOnly -FullScreen -UseLocalCursor=0 -PasswordFile $3"
            else
               $DBUS_HANDLER --auth=$4 --type=exec --text="vncviewer $2 -viewonly -fullscreen -passwd $3"
            fi
        fi
     ;;
     stopclient)
        if [ $STANDALONE = 0 ]; then
            killall -SIGKILL vncviewer
            # killall return != 0 if no proccess killed
            #if [ $? = 0 ]; then echo "ok"; else echo "error: killing vnc server"; fi
            echo "ok"
        else
            $DBUS_HANDLER --auth=$2 --type=exec --text="killall -s KILL vncviewer"
        fi
     ;;
     genpass)
        # create passwd file
        x11vnc -storepasswd "$2" "$3" > /dev/null 2>&1
        if [ $? = 0 ]; then echo "ok"; else echo "error"; fi
        # set read to all
        chmod go+r "$3"
     ;;
     *)
       echo "error: unknow action"
     ;;
  esac

done
