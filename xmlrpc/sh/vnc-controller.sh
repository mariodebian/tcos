#!/bin/sh

#
#  VNC controller script for TCOS
#
# examples:
#   vnc-controller.sh startserver "authfile"
#   vnc-controller.sh startserver "authfile" "user:pass"
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


DAEMONIZE="/sbin/start-stop-daemon -q -S -b -x "

export DISPLAY=:0

if [ -e /conf/tcos-run-functions ]; then
  # running in thin client
  STANDALONE=0
  export XAUTHORITY=/root/.Xauthority
else
  . /etc/tcos/tcos.conf
  STANDALONE=1
  STANDALONE_USER=$(who | awk '{ if ($2 == ":0") print $1 }' |tail -1)
  if [ "${STANDALONE_USER}" = "" ]; then echo "error: no standalone user connected"; exit 1; fi
  DBUS_HANDLER="${TCOS_BINS}/tcos-dbus-helper --username=${STANDALONE_USER} "
fi


for arg in $1; do
  case $arg in
     startserver)
        cmd=" -shared -noshm -viewonly -forever -rfbauth $1"
        if [ $STANDALONE = 0 ]; then
            $DAEMONIZE "x11vnc -- $cmd"
            if [ $? = 0 ]; then echo "ok"; else echo "error"; fi
        else
            # this returns ok if can send dbus msg, no need to parse
            $DBUS_HANDLER "--auth=$2 --type=exec --text='x11vnc $cmd"
        fi
     ;;
     stopserver)
        cmd="killall x11vnc"
        if [ $STANDALONE = 0 ]; then
           $cmd
           if [ $? = 0 ]; then echo "ok"; else echo "error"; fi
        else
           $DBUS_HANDLER "--auth=$1 --type=exec --text='$cmd'"
        fi
        
     ;;
     startclient)
        VNC_NEW_VERSION=$(vncviewer --version 2>&1| grep built |grep -c "4.1")
        if [ $VNC_NEW_VERSION = 1 ]; then
           $DAEMONIZE "vncviewer $2 -ViewOnly -FullScreen -passwd $3"
           if [ $? = 0 ]; then echo "ok"; else echo "error"; fi
        else
           $DAEMONIZE "vncviewer $2 -viewonly -fullscreen -passwd $3"
           if [ $? = 0 ]; then echo "ok"; else echo "error"; fi
        fi
     ;;
     stopclient)
        killall vncviewer
        if [ $? = 0 ]; then echo "ok"; else echo "error"; fi
     ;;
     genpass)
        # create passwd file
        x11vnc -storepasswd "$2" "$3" > /dev/null 2>&1
        if [ $? = 0 ]; then echo "ok"; else echo "error"; fi
        # set read to all
        chmod go+r "$3"
     ;;
     *)
       echo "error"
     ;;
  esac

done
