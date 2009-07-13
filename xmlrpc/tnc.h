/*
* tnc.h part of tcosxmlrpc
*   => common headers of tnc.c
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



/* xmlrpc methods of rtp  */
#define TNC_CONTROLLER TCOS_PATH"/tnc "
#define NETWORK_IFACE TCOS_PATH"/getinfo.sh -i NETWORK_IFACE"

#define TNC_ERROR "error: tnc command error"
#define TNC_OK "ok"


#if NEWAPI
xmlrpc_value *tcos_tnc(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext);
#else
xmlrpc_value *tcos_tnc(xmlrpc_env *env, xmlrpc_value *in, void *ud);
#endif
