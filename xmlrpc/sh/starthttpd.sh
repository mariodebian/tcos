#!/bin/sh

#/*
#* starthttpd.sh shell script to start a small web server in 8081 port
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


if [ -e /conf/tcos-run-functions ]; then
  _www=/var/www
  _conf=/etc/httpd2.conf
  _log=/var/log
else
  _www=/var/lib/tcos/standalone/www
  _conf=/var/lib/tcos/standalone/etc/httpd.conf
  _log=/var/lib/tcos/standalone/log
fi


_port=8081

mkdir -p ${_www}
mkdir -p ${_log}

echo "starting httpd..." >> ${_log}/httpd.log

if [ -f /bin/busybox-static ]; then
  echo "PXES/LTSP/Standalone running..." >> ${_log}/httpd.log
  busybox-static httpd -p ${_port} -h ${_www} -c ${_conf}  &
else
  echo "TCOS  running..." >> ${_log}/httpd.log
  busybox httpd -p ${_port} -h ${_www} -c ${_conf} &
fi

exit 0
