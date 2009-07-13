/*
* xorg.h part of tcosxmlrpc
*   => common headers of xorg.c
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



#define XORGCFG TCOS_PATH"/configurexorg"
#define XORG_CONF "/etc/X11/xorg.conf"

#define XORG_NEW_CONF   XORGCFG" --newsettings --outputfile="XORG_CONF
#define XORG_CHANGE_CONF   XORGCFG" --changesettings --outputfile="XORG_CONF" "
#define XORG_GET_CONF      XORGCFG" --getsettings "
#define XORG_REBUILD_CONF   XORGCFG" --newsettings --downloadagain --restartxorg --outputfile="XORG_CONF" "

/* FIXME */
#define XORG_MONITOR_NAME "grep \"Monitor name\"  /var/log/Xorg.0.log | awk -F \": \" '{print $3}'"

/* messages */
#define XORG_OK "ok"
#define XORG_ERROR "error: Command return error"
#define XORG_UNKNOW_OPTION "error: Unknow option for "XORGCFG
#define XORG_READING_ERROR "error: Error reading pipe"

#if NEWAPI
xmlrpc_value *tcos_xorg(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext);
#else
xmlrpc_value *tcos_xorg(xmlrpc_env *env, xmlrpc_value *in, void *ud);
#endif
