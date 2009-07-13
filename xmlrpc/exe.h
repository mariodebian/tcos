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
 #define XAUTHORITY "/usr/lib/tcos/get_xauth"
 #define DISPLAY "DISPLAY=:0 XAUTHORITY=$("XAUTHORITY")"
 #define CMD_WRAPPER DISPLAY" /usr/lib/tcos/daemonize.sh "
#else
 #define XAUTHORITY "/root/.Xauthority"
 #define DISPLAY "DISPLAY=:0 XAUTHORITY="XAUTHORITY
 #define CMD_WRAPPER DISPLAY" /sbin/daemonize.sh "
#endif


#if NEWAPI
xmlrpc_value *tcos_exe(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext);
#else
xmlrpc_value *tcos_exe(xmlrpc_env *env, xmlrpc_value *in, void *ud);
#endif

#if NEWAPI
xmlrpc_value *tcos_kill(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext);
#else
xmlrpc_value *tcos_kill(xmlrpc_env *env, xmlrpc_value *in, void *ud);
#endif

void job_exe( char *cmd );
void kill_exe( char *cmd );

