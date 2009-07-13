/*
* simple-methods.h part of tcosxmlrpc
*   => common headers of simple-methods.c
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


#if NEWAPI
xmlrpc_value *tcos_version(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext);
#else
xmlrpc_value *tcos_version(xmlrpc_env *env, xmlrpc_value *in, void *ud);
#endif

#if NEWAPI
xmlrpc_value *tcos_echo(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext);
#else
xmlrpc_value *tcos_echo(xmlrpc_env *env, xmlrpc_value *in, void *ud);
#endif

#if NEWAPI
xmlrpc_value *tcos_status(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext);
#else
xmlrpc_value *tcos_status (xmlrpc_env *env, xmlrpc_value *in, void *user_data);
#endif
