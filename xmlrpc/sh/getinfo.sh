#!/bin/sh
#
# getinfo.sh part of tcosxmlrpc
#   => shell script t return some /proc info
# Copyright (C) 2006-2011  mariodebian at gmail
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




IFCONFIG="busybox ifconfig"
# not DHCP try with static ip
for dev in $(ls /sys/class/net/|grep -v lo|grep -v sit0|grep -v vmnet|grep -v wmaster|grep -v vbox); do
     LINK=$(cat /sys/class/net/$dev/carrier 2>/dev/null)
     if [ "$LINK" = "1" ] && \
        [ "$(busybox ifconfig $dev | grep 'inet addr:'| awk '{print $2}'| awk -F ":" '{print $2}')" != "" ]; then
            IFDEV=$dev
            break
     fi
done

if [ "$IFDEV" = "" ]; then
  IFDEV="eth0"
fi

if [ -d "/sys/class/net/tun0" ]; then
  # use tun0 if openvpn is running
  IFDEV="tun0"
fi

if [ -d "/sys/class/net/tap0" ]; then
  # use tun0 if openvpn is running
  IFDEV="tap0"
fi


 KVER=$(uname -r)
 TCOS_CONF=/conf/tcos.conf
 STANDALONE=0
if [ ! -e ${TCOS_CONF} ]; then
 TCOS_CONF=/var/lib/tcos/standalone/etc/tcos.conf
 STANDALONE=1
 export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/lib/tcos
fi

usage() {
  echo "getinfo.sh help"
  echo "     -i VARNAME    - show value of VARNAME"
  echo "     -l            - show list of all vars"
  echo "     -d NET_DEVICE - change net device, default(${IFDEV})"
  echo "     -t            - get thin client type (tcos, pxes, ltsp, unknow)"
  echo "     -p            - list all process running"
  echo "     -h (this help)"

}

if [ $# -lt 1 ]; then
  usage
  exit 0
fi

show_vars() {
awk '/^CPU/     { gsub("\)", "", $1) ; print $1}' $0 2>/dev/null
awk '/^RAM/     { gsub("\)", "", $1) ; print $1}' $0 2>/dev/null
awk '/^SWAP/    { gsub("\)", "", $1) ; print $1}' $0 2>/dev/null
awk '/^TCOS/    { gsub("\)", "", $1) ; print $1}' $0 2>/dev/null
awk '/^KERNEL/  { gsub("\)", "", $1) ; print $1}' $0 2>/dev/null
awk '/^NET/     { gsub("\)", "", $1) ; print $1}' $0 2>/dev/null
awk '/^MODULES/ { gsub("\)", "", $1) ; print $1}' $0 2>/dev/null
}

get_type() {
if [ -f ${TCOS_CONF} ]; then
 if [ "${STANDALONE}" = "0" ]; then
   echo "tcos"
 else
   echo "standalone"
 fi
elif [ -f /var/run/pxes/env ]; then
  echo "pxes"
elif [ -f /etc/lts.conf ]; then
  echo "ltsp"
else
  echo "unknow"
fi
}


modules_notfound() {
 #discover_modules=$(grep Discover /tmp/initramfs.debug 2>/dev/null | awk -F ":" '{print $2}')
 discover_modules=$(/bin/pcimodules 2> /dev/null | grep -E -v '^ *$')
 loaded_modules=$(lsmod| grep -v ^Module| awk '{print $1}'| sed s/'\n'/''/g)

 loaded=""
 not_found=""
  for mod in ${loaded_modules}; do
    loaded="${loaded} ${mod}"
  done

  for mod in ${discover_modules}; do
    #mod2=$(echo ${mod} | sed s/"-"/"_"/g)
    #if [ $(grep -c ${mod2} /proc/modules) = 0 ]; then
    #  not_found="${not_found} ${mod2}"
    #fi
    if [ ! -f "$(find /lib/modules/${KVER}/ -name "$mod.ko")" ]; then
      not_found="${not_found} ${mod}"
    fi
  done
 if [ "${not_found}" != "" ]; then
  echo "${not_found}"
 else
  echo "OK"
 fi
}

get_line() {
echo $(head -$1 /tmp/ps.aux | tail -1)
}

get_process() {
if [ "$STANDALONE" = "1" ]; then
  #user=$(w | awk '{ if ($3 == ":0" || $2 == ":0") print $1 }')
  user=$(/usr/lib/tcos/tcos-last --userid 2>/dev/null)
  if [ "$user" = "root" ]; then
    echo "PID COMMAND" > /tmp/ps.aux
    echo "66000 User root not allowed to show process" >> /tmp/ps.aux
  else
    LANG=C ps U ${user} -o pid,command | /usr/lib/tcos/clean_string.sh > /tmp/ps.aux
  fi
else
  # FIXME Thin client need clean_string.sh???
  ps aux > /tmp/ps.aux
fi
num_lines=$(cat /tmp/ps.aux | wc -l)
for i in $(seq 1 $num_lines); do
  echo -n "$(get_line $i)|"
done
echo
rm -f /tmp/ps.aux
}

#while getopts "d:i:hv:o:p:m:" options
while getopts "tpld:i:h" options
do
        case $options in
                h) usage
                        exit 0
                        ;;
                i) VARNAME=$OPTARG
                        ;;
                d) IFDEV=$OPTARG
                        ;;
                l) show_vars
                        ;;
                t) get_type
                        ;;
                p) get_process
                        ;;
                *) usage
			exit 0
                        ;;

	esac
done

IFCMD="${IFCONFIG} ${IFDEV} "


case $VARNAME in

# CPU methods 
CPU_MODEL)
grep "^model name" /proc/cpuinfo | awk -F ": " '{print $2}' |head -1
;;

CPU_SPEED)
grep "^cpu MHz" /proc/cpuinfo | awk -F ": " '{print $2" MHz"}' | head -1
;;

CPU_VENDOR)
grep "^vendor_id" /proc/cpuinfo | awk -F ": " '{print $2}' | head -1
;;

# RAM methods
RAM_TOTAL)
grep ^MemTotal /proc/meminfo | awk '{print $2" "$3}'
;;

RAM_ACTIVE)
grep ^Active /proc/meminfo | awk '{print $2" "$3}'
;;

RAM_FREE)
busybox free| grep "Mem:"| awk '{print $4" kB"}'
;;

RAM_USED)
busybox free| grep "Mem:"| awk '{print $3" kB"}'
;;

# SWAP methods
SWAP_AVALAIBLE)
grep -c ^/ /proc/swaps
;;

SWAP_TOTAL)
busybox free| grep "Swap:"| awk '{print $2" kB"}'
;;

SWAP_USED)
busybox free| grep "Swap:"| awk '{print $3" kB"}'
;;

SWAP_FREE)
busybox free| grep "Swap:"| awk '{print $4" kB"}'
;;

# DATE and version methods
TCOS_DATE)
LANG=C date
;;

TCOS_GENERATION_DATE)
grep ^TCOS_DATE ${TCOS_CONF}| awk -F "=" '{print $2}' | sed s/'"'//g
;;

TCOS_VERSION)
grep ^TCOS_VERSION ${TCOS_CONF}| awk -F "=" '{print $2}' |sed s/'"'//g
;;

TCOS_UPTIME)
uptime
;;

# KERNEL methods
KERNEL_VERSION)
busybox uname -r
;;

KERNEL_COMPLETE_VERSION)
busybox uname -a
;;

# NETWORK methods 
NETWORK_HOSTNAME)
hostname
;;

NETWORK_IP)
${IFCMD} | grep "inet addr:"| awk '{print $2}'| awk -F ":" '{print $2}'
;;

NETWORK_IFACE)
echo ${IFDEV}
;;

NETWORK_MAC)
${IFCMD} | grep "HWaddr"| awk '{print $5}'
;;

NETWORK_MASK)
${IFCMD} | grep "inet addr:"| awk '{print $4}'| awk -F ":" '{print $2}'
;;

NETWORK_RX)
${IFCMD} |grep "RX bytes"| awk '{print $3" "$4}'| sed s/'('//g | sed s/')'//g
;;

NETWORK_TX)
${IFCMD} |grep "TX bytes"| awk '{print $7" "$8}'| sed s/'('//g | sed s/')'//g
;;

# MODULES methods 
MODULES_LOADED)
for module in $(cat /proc/modules| awk '{print $1}'); do 
 echo -n "$module "
done
echo
;;

MODULES_NOTFOUND)
modules_notfound
;;

SCREENSIZE)
DISPLAY=:0 screensize
;;
esac




