# -------
# File:        session-cmd-common.sh
# Description: common functions for session-cmd scripts
# Author:      Luis Garcia Gisbert <garcia_luigis@gva.es>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston MA 02110-1301 USA
# --------

UNIX_SOCKET_DIR="/tmp/session-cmd"

socket_basename(){
   echo "$(id -un)"
}

create_socket(){
   local i MAX_TRY UNIX_SOCKET
   i=0
   MAX_TRY=99
   while [ $i -lt $MAX_TRY ] ; do
      UNIX_SOCKET="${UNIX_SOCKET_DIR}-$(socket_basename)/$(socket_basename)$-{i}"
      [ -S "${UNIX_SOCKET}.sock" ] || break
      if [ -r "${UNIX_SOCKET}.pid" ] ; then
         kill -9 "$(cat "${UNIX_SOCKET}.pid")" >/dev/null 2>/dev/null || true
         break
      fi
      i=$(($i + 1))
   done
   [ $i -lt $MAX_TRY ] || return 1
   mkdir -p "${UNIX_SOCKET_DIR}-$(socket_basename)"
   [ -d "${UNIX_SOCKET_DIR}-$(socket_basename)" ] || return 1
   echo "$UNIX_SOCKET"
   return 0
}

list_sockets(){
   [ -d "${UNIX_SOCKET_DIR}-$(socket_basename)" ] || return 0
   find "${UNIX_SOCKET_DIR}-$(socket_basename)" -type s -name "$(socket_basename)-*.sock" 2>/dev/null
   return 0
}
