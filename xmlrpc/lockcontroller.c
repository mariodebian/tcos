/*
* lock-controller.c part of tcosxmlrpc
*   => method that lock unlock and get status of screen
* Copyright (C) 2008  vidal_joshur at gva.es
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
#include "exe.h"

void 
lockcontroller_exe( char *action )
{
  char cmd[BIG_BUFFER];
  dbgtcos("tcosxmlrpc::lockcontroller_exe() action=> \"%s\"\n", action);

  if ( strcmp(TCOS_PATH, "/sbin" ) )
    sprintf( cmd , "%s/%s", TCOS_PATH, action );
  else
    sprintf( cmd , "%s", action );

  job_exe(cmd);
  return;
}

void 
lockcontroller_kill( char *action )
{
  dbgtcos("tcosxmlrpc::lockcontroller_kill() action=> \"%s\"\n", action);

  kill_exe(action);

  return;
}

#if NEWAPI
xmlrpc_value *tcos_lockcontroller(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_lockcontroller(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  char *user;
  char *pass;
  char *action;
  char *login_ok;
  char cmd[BIG_BUFFER];
   /* Parse app string */
   xmlrpc_parse_value(env, in, "(sss)", &action, &user, &pass);
   if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

  /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  if ( strcmp(TCOS_PATH, "/sbin" ) )
    sprintf( cmd , "%s/%s", TCOS_PATH, action );
  else
    sprintf( cmd , "%s", action );

  job_exe(cmd);

  return xmlrpc_build_value(env, "s", "OK" );
}

#if NEWAPI
xmlrpc_value *tcos_unlockcontroller(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_unlockcontroller(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  char *user;
  char *pass;
  char *action;
  char *login_ok;

   /* Parse app string */
   xmlrpc_parse_value(env, in, "(sss)", &action, &user, &pass);
   if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  kill_exe(action);
  return xmlrpc_build_value(env, "s", "OK" );
}


