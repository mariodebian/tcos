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

export DISPLAY=:0

if [ -e /conf/tcos-run-functions ]; then
  # running in thin client
  STANDALONE=0
  export XAUTHORITY=/root/.Xauthority
else
  . /etc/tcos/tcos.conf
  STANDALONE=1
  STANDALONE_USER=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }' |head -1)
  if [ "${STANDALONE_USER}" = "" ]; then echo "error: no standalone user connected"; exit 1; fi
  DBUS_HANDLER="${TCOS_BINS}/tcos-dbus-helper --username=${STANDALONE_USER} "
fi


for arg in $1; do
  case $arg in
     startserver)
        cmd=" -shared -noshm -forever -rfbauth $2"
        if [ $STANDALONE = 0 ]; then
             killall x11vnc
            /sbin/daemonize.sh "/usr/bin/x11vnc" "$cmd"
            if [ $? = 0 ]; then echo "ok"; else echo "error: starting vnc server"; fi
        else
            $DBUS_HANDLER --auth=$2 --type=exec --text="killall x11vnc"
            # this returns ok if can send dbus msg, no need to parse
            $DBUS_HANDLER --auth=$3 --type=exec --text="x11vnc $cmd"
        fi
     ;;
     stopserver)
        if [ $STANDALONE = 0 ]; then
           killall x11vnc
           if [ $? = 0 ]; then echo "ok"; else echo "error: killing vnc server"; fi
        else
           $DBUS_HANDLER --auth=$2 --type=exec --text="killall x11vnc"
        fi
        
     ;;
     startclient)
        # exit if pass file not exists
        if [ ! -f "$3" ]; then echo "error: no passwd file"; exit 1; fi
        
        # check vncviewer version
        
        VNC_NEW_VERSION=$(vncviewer --version 2>&1| grep built |grep -c "4.1")
        if [ $VNC_NEW_VERSION = 1 ]; then
           /sbin/daemonize.sh "/usr/bin/vncviewer" "$2 -ViewOnly -FullScreen -passwd $3"
           if [ $? = 0 ]; then echo "ok"; else echo "error: starting vncviewer"; fi
        else
           /sbin/daemonize.sh "/usr/bin/vncviewer" "$2 -viewonly -fullscreen -passwd $3"
           if [ $? = 0 ]; then echo "ok"; else echo "error: starting vncviewer"; fi
        fi
     ;;
     stopclient)
        killall vncviewer
        if [ $? = 0 ]; then echo "ok"; else echo "error: killing vncviewer"; fi
     ;;
     genpass)
        # create passwd file
        /usr/bin/x11vnc -storepasswd "$2" "$3" > /dev/null 2>&1
        if [ $? = 0 ]; then echo "ok"; else echo "error"; fi
        # set read to all
        chmod go+r "$3"
     ;;
     *)
       echo "error"
     ;;
  esac

done
