/*
* config.h part of tcosxmlrpc
*   => common headers of config.c
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

#define CONFIG_WRAPPER TCOS_PATH"/configctl.sh "

#define CONFIG_OK    "ok: config action done."
#define CONFIG_ERROR "-1"

#define CONFIG_READING_ERROR "error: reading config settings."
#define CONFIG_UNKNOW_ERROR "error: unknow option passed."


#if NEWAPI
xmlrpc_value *tcos_config(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext);
#else
xmlrpc_value *tcos_config(xmlrpc_env *env, xmlrpc_value *in, void *ud);
#endif
