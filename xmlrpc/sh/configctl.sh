#!/bin/sh
# TCOS parse config vars
#/*
#* configctl.sh part of tcosxmlrpc
#*   => get TCOS config vars
#* Copyright (C) 2006,2007,2008  mariodebian at gmail
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


get_varname() {
  if [ -e /conf/tcos.conf ]; then
    # don't allow to ask for root passwd
    if [ "$1" = "TCOS_ROOT_PASSWD" ]; then
      echo "*secret*"
      return
    fi
    
    line=$(grep "^${1}=" /conf/tcos.conf | tail -1)
    if [ "$line" != "" ]; then
      echo $line | awk -F"=" '{print $2}'
    else
      echo "-1"
    fi
  fi
}


usage() {
  echo "Usage:"
  echo "       $0  --help                  ( this help text )"
  echo "       $0  --get [VARNAME]         ( return 0/1 if found )"
}



for x in $1; do
	case $x in
    --get)
		output=$(get_varname "$2")
		;;
    --help)
		usage
		exit 1
		;;
    esac
done




if [ "$1" = "" ]; then
  usage
  exit 1
fi

if [ "$output" = "" ]; then
  output="0"
fi

echo -n $output

exit 0
