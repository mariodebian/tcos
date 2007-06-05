#!/bin/sh

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
 # start Xorg at tty7 to avoid getty keyboard conflicts
 Xorg tty7 ${TCOS_XORG_OPTS} > /dev/null 2>&1
 counter=$((counter+1))
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
# start Xorg at tty7 to avoid getty keyboard conflicts
Xorg tty7 -config /etc/X11/xorg.conf.auto ${TCOS_XORG_OPTS}  > /dev/null 2>&1

echo ""
echo ""
echo ""
echo "Error launching Xorg, check /var/log/Xorg.0.log for errors"
echo ""
echo "   Is font server (xfs) running at ${FONT_SERVER}?"
echo ""
echo "This is the last lines of Xorg.0.log"
echo ""
tail /var/log/Xorg.0.log
echo ""

exit 1
