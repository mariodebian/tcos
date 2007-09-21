/*exe.h common headers  2006-09-09 14:22:40 mariodebian $
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
*/

#ifdef IS_STANDALONE
 #define DISPLAY "DISPLAY=:0 XAUTHORITY=/home/$("STANDALONE_USER")/.Xauthority"
#else
 #define DISPLAY "DISPLAY=:0"
#endif
/*
#define CMD_WRAPPER DISPLAY" /sbin/start-stop-daemon --start --quiet --background --exec "
*/

/* #define CMD_WRAPPER DISPLAY" nohup " */
/* #define CMD_WRAPPER DISPLAY" busybox-static start-stop-daemon -S -b -x " */

#define CMD_WRAPPER DISPLAY" /sbin/start-stop-daemon -q -S -b -x "

/*#define CMD_WRAPPER DISPLAY" /bin/daemonize "*/


