/*
* vnc.c part of tcosxmlrpc
*   => method to start/stop vnc server and client
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

#include "vnc.h"


#if NEWAPI
static xmlrpc_value *tcos_vnc(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
static xmlrpc_value *tcos_vnc(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char line[BIG_BUFFER];
  char *action;
  char *args;
  char *user;
  char *pass;
  char *login_ok;
  char cmd[BIG_BUFFER];


  /* read what info search */
  xmlrpc_parse_value(env, in, "(ssss)", &action, &args, &user, &pass);
  if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  dbgtcos("tcosxmlrpc::tcos_vnc() login ok, action=%s, args=%s, user=%s, pass=**notshow**\n", action, args, user);

#ifdef IS_STANDALONE
  dbgtcos("tcosxmlrpc::tcos_vnc() standalone\n");
#else
  dbgtcos("tcosxmlrpc::tcos_vnc() thin client\n");
#endif

  /* prepare action */
  sprintf( cmd , "%s %s %s %s:%s", VNC_CONTROLLER, action, args, user, pass );

  dbgtcos("tcosxmlrpc::tcos_vnc() cmd=\"%s\"\n", cmd);

  fp=(FILE*)popen(cmd, "r");

  /* put error in line */
  strncpy(line, VNC_ERROR, BIG_BUFFER);

  fgets( line, sizeof line, fp);
  remove_line_break(line);
  pclose(fp);
  dbgtcos("tcosxmlrpc::tcos_vnc() line=\"%s\"", line);

  if( strcmp(action, "startclient" ) == 0 )
    lockcontroller_exe("lockvnc");
  else if( strcmp(action, "stopclient" ) == 0 )
    lockcontroller_kill("lockvnc");

  return xmlrpc_build_value(env, "s", line );
}


