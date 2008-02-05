/*
* lockkeybmouse.c part of tcosxmlrpc
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



#if NEWAPI
static xmlrpc_value *tcos_lockkeybmouse(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
static xmlrpc_value *tcos_lockkeybmouse(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  char *user;
  char *pass;
  char *login_ok;
  
   /* Parse app string */
   xmlrpc_parse_value(env, in, "(ss)", &user, &pass);
   if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

  /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  if ( strcmp(TCOS_PATH, "/sbin" ) )
    job_exe(TCOS_PATH"/lockkeybmouse");

  else
    job_exe("lockkeybmouse");

  return xmlrpc_build_value(env, "s", "OK" );
}

#if NEWAPI
static xmlrpc_value *tcos_unlockkeybmouse(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
static xmlrpc_value *tcos_unlockkeybmouse(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  char *user;
  char *pass;
  char *login_ok;

   /* Parse app string */
   xmlrpc_parse_value(env, in, "(ss)", &user, &pass);
   if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  system("killall lockkeybmouse");
  return xmlrpc_build_value(env, "s", "OK" );
}


