/*# login.c method that AUTH remote XMLRPC 2006-09-09 14:22:40 mariodebian $
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

#include "login.h"
#include "validate.c"





static xmlrpc_value *
tcos_login(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {
/*  FILE *fp;*/
  char *login_ok;
  char *user;
  char *pass;
  

  xmlrpc_parse_value(env, in, "(ss)", &user, &pass);

  dbgtcos("tcosxmlrpc::tcos_login() user=\"%s\" pass=\"******\"\n");

  /*fprintf(stderr, "tcosxmlrpc::tcos_login() user=\"%s\" pass=\"%s\"\n", user, pass);*/



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



static xmlrpc_value *
tcos_logout(xmlrpc_env *env, xmlrpc_value *in, void *ud)
{
/*   return xmlrpc_build_value(env, "s", tcos_logout_helper() );*/
   return xmlrpc_build_value(env, "s", 1 );
}



