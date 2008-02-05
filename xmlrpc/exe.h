/*
* exe.h part of tcosxmlrpc
*   => common headers of exe.c
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


#ifdef IS_STANDALONE
 #define DISPLAY "DISPLAY=:0 XAUTHORITY=/home/$("STANDALONE_USER")/.Xauthority"
 #define CMD_WRAPPER DISPLAY" /usr/lib/tcos/daemonize.sh "
#else
 #define DISPLAY "DISPLAY=:0 XAUTHORITY=/root/.Xauthority"
 #define CMD_WRAPPER DISPLAY" /sbin/daemonize.sh "
#endif


