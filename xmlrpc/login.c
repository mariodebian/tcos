/*
* login.c part of tcosxmlrpc
*   => method that AUTH remote XMLRPC
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

#include "common.h"
#include "debug.h"
#include "validate.h"
#include "login.h"


#if NEWAPI
xmlrpc_value *tcos_login(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_login(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  char *login_ok;
  char *user;
  char *pass;


  xmlrpc_parse_value(env, in, "(ss)", &user, &pass);

  dbgtcos("tcosxmlrpc::tcos_login() user=\"%s\" pass=\"******\"\n");

  login_ok=validate_login(user, pass);

  /*  if ( login_ok == LOGIN_OK )*/
  if ( strcmp(login_ok,  LOGIN_OK ) == 0 )
    return xmlrpc_build_value(env, "s", LOGIN_OK_MSG);

  /*  else if ( login_ok == LOGIN_NOUSER) */
  else if ( strcmp(login_ok,  LOGIN_NOUSER ) == 0 )
    return xmlrpc_build_value(env, "s", LOGIN_NOUSER_MSG);

  /*  else if ( login_ok == LOGIN_NOPASS) */
  else if (  strcmp(login_ok,  LOGIN_NOPASS ) == 0 )
    return xmlrpc_build_value(env, "s", LOGIN_NOPASS_MSG);

  /*  else if ( login_ok == LOGIN_NOPERMS) */
  else if (  strcmp(login_ok,  LOGIN_NOPERMS ) == 0 )
    return xmlrpc_build_value(env, "s", LOGIN_NOPERMS_MSG);

  /*  else if ( login_ok == LOGIN_EXPIRED) */
  else if (  strcmp(login_ok,  LOGIN_EXPIRED ) == 0 )
    return xmlrpc_build_value(env, "s", LOGIN_EXPIRED_MSG);

  return xmlrpc_build_value(env, "s", LOGIN_ERROR);
}


#if NEWAPI
xmlrpc_value *tcos_logout(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_logout(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
{
   /* dummy method */
   return xmlrpc_build_value(env, "s", 1 );
}



