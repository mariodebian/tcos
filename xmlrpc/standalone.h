/*
* standalone.h part of tcosxmlrpc
*   => common headers of standalone.c
* Copyright (C) 2006,2007,2008  mariodebian at gmail
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


/* xmlrpc methods to export thin client info */

/*#define STANDALONE_USER    "w | awk '{ if ($3 == \":0\" || $2 == \":0\") print $1 }' |head -1"*/
#define STANDALONE_USER    "/usr/lib/tcos/tcos-last --user 2>/dev/null"
#define STANDALONE_HOME    "getent passwd | grep \"^$("STANDALONE_USER"):\" | awk -F\":\" '{print $6}'"
#define STANDALONE_PROCESS "ps U $("STANDALONE_USER") -o pid | sed 's/[[:blank:]]//g' | grep -c ^[0-9]"
/*#define STANDALONE_TIME    "LC_ALL=C LC_MESSAGES=C last| grep \"[[:blank:]]:0[[:blank:]].*still\" | awk '{print $(NF-5)\" \"$(NF-4)\" \"$(NF-3)}'"*/
#define STANDALONE_TIME    "/usr/lib/tcos/tcos-last --time 2>/dev/null"

#ifdef IS_STANDALONE
  #define STANDALONE_SERVER  "tail -1 /var/lib/tcos/standalone/log/access.log | awk '{print $1}'"
#else
  #define STANDALONE_SERVER  "tail -1 /var/log/access.log | awk '{print $1}'"
#endif

/* messages */
#define STANDALONE_UNKNOW "error: Unknow user"
#define STANDALONE_ERROR  "error: tcos-last returned error"


/* dbus */
#define DBUS_HANDLER TCOS_PATH "/tcos-dbus-helper"

#define DBUS_ERROR "error: dbus error"
#define DBUS_OK    "ok"




