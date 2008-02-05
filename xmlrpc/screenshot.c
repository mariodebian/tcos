/*
* screenshot.c part of tcosxmlrpc
*   => method exec scrot to make an screenshot
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

#include "screenshot.h"


#if NEWAPI
static xmlrpc_value *tcos_screenshot(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
static xmlrpc_value *tcos_screenshot(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  char line[BIG_BUFFER];
  char *size;
  char *user;
  char *pass;
  char *login_ok;

  /* read what info search */
  xmlrpc_parse_value(env, in, "(sss)", &size, &user, &pass);
  if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  if ( strlen(size) > 0)
     snprintf( (char*) line, BSIZE, "%s %s", SCROT_CMD, size);
  else
     snprintf( (char*) line, BSIZE, "%s", SCROT_CMD);

  dbgtcos("tcosxmlrpc::screenshot() exe=%s\n", line);

  if (system(line) != 0 )
      return xmlrpc_build_value(env, "s", SCROT_ERROR );
   else
      return xmlrpc_build_value(env, "s", SCROT_OK );

}

