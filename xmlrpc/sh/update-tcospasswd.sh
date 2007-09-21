#!/bin/bash

# update-tcospasswd.sh shell script to generate tcospasswd
#                      to PXES, LTSP or TCOS
#  2006-09-09 14:22:40 mariodebian $
#
# This file is part of tcosxmlrpc.
#
# tcosxmlrpc is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# tcosxmlrpc is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with tcosxmlrpc; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.

if [ "$(whoami)" != "root" ]; then
  echo "Error: Need to exec as root user."
  exit 1
fi

_is_ltsp=$(echo /opt/ltsp*)
_is_pxes=$(echo /opt/pxes*)
_is_tcos=$(echo /etc/tcos)

if [ -d ${_is_ltsp} ]; then
  ltsp=1
fi
if [ -d ${_is_pxes} ]; then
  pxes=1
fi
if [ -d ${_is_tcos} ]; then
  tcos=1
fi


make_passwd(){
$(which tcospasswd)
}


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
  fpass=/opt/ltsp/i386/etc/tcospasswd

elif [ "$_arq" = "PXES" ]; then
  fpass=$(echo /opt/pxes*)/stock/dist/etc/tcospasswd

elif [ "$_arq" = "TCOS" ]; then
  fpass=/etc/tcospasswd

elif [ "$_arq" = "STANDALONE" ]; then
  fpass=/etc/tcospasswd
else
  echo "Error: Arquitecture: ${_arq} not supported, please write LTSP, PXES, TCOS or STANDALONE"
  exit 1
fi

echo -n "Username (usually root): "
read _username

string="$_username:$(make_passwd)"


if [ ! -d $(dirname ${fpass}) ]; then 
  echo "Error: $(dirname ${fpass}) don't exists."
  exit 1
fi

echo -n "I will write this file:
  ${fpass}
          Continue? [Y/n] "
read _ans

if [ "${_ans}" != "n" ]; then
  echo ${string} > ${fpass}
  echo ""
  echo "In case of PXES or TCOS (not STANDALONE), please update the"
  echo "boot image with pxesconfig or gentcos."
  echo "Done."
  exit 0
fi

echo "Aborted."
exit 1

