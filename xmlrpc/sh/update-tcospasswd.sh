#!/bin/bash
#
# update-tcospasswd.sh shell script to generate tcospasswd
#                      to PXES, LTSP, TCOS or STANDALONE
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



if [ "$(whoami)" != "root" ]; then
  echo "Error: Need to exec as root user."
  exit 1
fi

_is_ltsp=$(echo /opt/ltsp*)
_is_pxes=$(echo /opt/pxes*)
_is_tcos=$(echo /etc/tcos)
_username=""
_passwd=""
_arq=""
_fpass=""
_silent=0
_noencrypted=0
_secret=0

if [ -d ${_is_ltsp} ]; then
  ltsp=1
fi
if [ -d ${_is_pxes} ]; then
  pxes=1
fi
if [ -d ${_is_tcos} ]; then
  tcos=1
fi

for x in $@; do
    case $x in
	--user=*)
		_username="${x#--user=}"
		;;
	--passwd=*)
		_passwd="${x#--passwd=}"
		;;
	--arq=*)
		_arq="${x#--arq=}"
		;;
	--file=*)
		_fpass=${x#--file=}
		;;
	--silent)
		_silent=1
		;;
	--noencrypted)
		_noencrypted=1
		;;
	--tcosmonitor-secret)
		_secret=1
		;;
    esac
done


make_passwd(){
$(which tcospasswd) $1
}


if [ -z $_arq ] && [ -z $_fpass ] && [ $_secret -eq 0 ]; then
   echo "Arquitecture to update password:"
   echo ""
   if [ ${ltsp} ]; then
     echo "  * LTSP"
   fi

   if [ ${pxes} ]; then
     echo "  * PXES"
   fi

   if [ ${tcos} ]; then
     echo "  * TCOS"
   fi


   echo "  * STANDALONE"

   echo -n "              : "
   read _arq

   if [ "$_arq" = "LTSP" ]; then
     _fpass=/opt/ltsp/i386/etc/tcospasswd

   elif [ "$_arq" = "PXES" ]; then
     _fpass=$(echo /opt/pxes*)/stock/dist/etc/tcospasswd

   elif [ "$_arq" = "TCOS" ]; then
     _fpass=/etc/tcospasswd

   elif [ "$_arq" = "STANDALONE" ]; then
     _fpass=/etc/tcospasswd
   else
     echo "Error: Arquitecture: ${_arq} not supported, please write LTSP, PXES, TCOS or STANDALONE"
     exit 1
   fi
fi


if [ -z $_username ];then
  echo -n "Username (usually root): "
  read _username
fi

if [ -z $_passwd ];then
  echo -n "TCOS Password: "
  read _passwd
fi

if [ $_secret -eq 1 ];then
  _fpass=/etc/tcos/secrets/tcosmonitor-secret
  [ ! -d $(dirname ${_fpass}) ] && exit 0
  echo "$_username:$_passwd" > ${_fpass}
  chown -f root:tcos ${_fpass}
  chmod -f 640 ${_fpass}
  exit 0
fi

if [ $_noencrypted -eq 0 ];then
  _passwd="$(make_passwd $_passwd)"
fi

string="$_username:$_passwd"

if [ $_silent -eq 0 ];then
  if [ ! -d $(dirname ${_fpass}) ]; then 
    echo "Error: $(dirname ${_fpass}) don't exists."
    exit 1
  fi

  echo -n "I will write this file:
    ${_fpass}
            Continue? [Y/n] "
  read _ans

  if [ "${_ans}" != "n" ]; then
    echo ${string} > ${_fpass}
    chown -f root:tcos ${_fpass}
    chmod -f 640 ${_fpass}
    echo ""
    echo "In case of PXES or TCOS (not STANDALONE), please update the"
    echo "boot image with pxesconfig or gentcos."
    echo "Done."
    exit 0
  fi
else
  echo ${string} > ${_fpass}
  chown -f root:tcos ${_fpass}
  chmod -f 640 ${_fpass}
  exit 0
fi

echo "Aborted."
exit 1

