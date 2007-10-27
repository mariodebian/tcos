/*# vnct.c method to start/stop vnc server and client 2007-10-24 09:43:40 mariodebian $
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

  /* prepare action */
#ifndef IS_STANDALONE
  dbgtcos("tcosxmlrpc::tcos_vnc() standalone\n");
  sprintf( cmd , "%s %s %s", VNC_CONTROLLER, action, args );
#else
  dbgtcos("tcosxmlrpc::tcos_vnc() thin client\n");
  sprintf( cmd , "%s %s %s %s:%s", VNC_CONTROLLER, action, args, user, pass );
#endif

  dbgtcos("tcosxmlrpc::tcos_vnc() cmd=\"%s\"\n", cmd);
  
  fp=(FILE*)popen(cmd, "r");

  /* put error in line */
  sprintf(line , "%s", VNC_ERROR);  
  
  fgets( line, sizeof line, fp);
  dbgtcos("tcosxmlrpc::tcos_vnc() line=\"%s\"", line);

  pclose(fp);
  return xmlrpc_build_value(env, "s", line );
}




