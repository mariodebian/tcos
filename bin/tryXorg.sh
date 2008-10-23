#!/bin/sh

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
