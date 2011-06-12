/*
* tnc.c part of tcosxmlrpc
*   => method to lock/unlock internet with iptables rules
* Copyright (C) 2006,2007,2008  mariodebian at gmail
* Copyright (C) 2006,2007,2008  vidal_joshur at gva.es
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
#include "tnc.h"


#if NEWAPI
xmlrpc_value *tcos_tnc(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_tnc(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char line[BIG_BUFFER];
  char *action;
  char *ports;
  char *onlyports;
  char *username;
  char *user;
  char *pass;
  char *login_ok;
  char cmd[BIG_BUFFER];
  /*char *fret;*/


  /* read what info search */
  xmlrpc_parse_value(env, in, "(ssssss)", &action, &onlyports, &ports, &username, &user, &pass);
  if (env->fault_occurred)
    return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  dbgtcos("tcosxmlrpc::tcos_tnc() login ok, action=%s, only-ports=%s, ports=%s, username=%s, user=%s, pass=**notshow**\n", action, onlyports, ports, username, user);

  /* prepare action */
  if( strcmp(action, "disable-internet" ) == 0 ) {
    if ((fp=(FILE*)popen(NETWORK_IFACE, "r")) != NULL) {
      (void)fgets( line, sizeof line, fp);
      remove_line_break(line);
       pclose(fp);
       sprintf( cmd , "%s %s %s %s %s %s", TNC_CONTROLLER, action, onlyports, ports, line, username);
    }
  }
  else {
    sprintf( cmd , "%s %s %s %s %s", TNC_CONTROLLER, action, onlyports, ports, username);
  }

  dbgtcos("tcosxmlrpc::tcos_tnc() cmd=\"%s\"\n", cmd);

  fp=(FILE*)popen(cmd, "r");

  /* put error in line */
  strncpy(line, TNC_ERROR, BIG_BUFFER);

  (void)fgets( line, sizeof line, fp);
  remove_line_break(line);
  pclose(fp);

  dbgtcos("tcosxmlrpc::tcos_tnc() line=\"%s\"\n", line);

  return xmlrpc_build_value(env, "s", line );
}


