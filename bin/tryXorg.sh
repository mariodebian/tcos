#!/bin/sh

. /conf/tcos.conf

# try to launch Xorg or return error

MAX_XORG_RETRY=3

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

# add font server
sed "/modules/i\
        FontPath        \"tcp/${FONT_SERVER}:7100\"\
" $new_xorg > /etc/X11/xorg.conf.auto

# change keyboard map
sed -i "/kbd/i\
	Option          "XkbRules"      "xorg"
        Option          "XkbModel"      "${TCOS_XORG_XKBMODEL}"
        Option          "XkbLayout"     "${TCOS_XORG_XKB}"
" /etc/X11/xorg.conf.auto


# start Xorg at vt7 to avoid getty keyboard conflicts
Xorg vt7 -config /etc/X11/xorg.conf.auto ${TCOS_XORG_OPTS}  > /dev/null 2>&1

cat /dev/null > /dev/tty1
chvt 1
echo ""
echo ""
echo ""
echo "Error launching Xorg, check /var/log/Xorg.0.log for errors"
echo ""
echo "   Is font server (xfs) running at ${FONT_SERVER}?"
echo ""
echo "   These are the lasts lines of Xorg.0.log"
echo ""
tail /var/log/Xorg.0.log
echo "---------------------------------------------------"
grep EE /var/log/Xorg.0.log
echo ""

exit 1
