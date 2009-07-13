/*
* dpms.c part of tcosxmlrpc
*   => method to set dpms monitor status
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
#include "exe.h"
#include "dpms.h"



#if NEWAPI
xmlrpc_value *tcos_dpms(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_dpms(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char line[BIG_BUFFER];
  char *action;
  char *user;
  char *pass;
  char *login_ok;
  char cmd[BIG_BUFFER];
  char *fret;


  /* read what info search */
  xmlrpc_parse_value(env, in, "(sss)", &action, &user, &pass);
  if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  dbgtcos("tcosxmlrpc::tcos_dpms() login ok, action=%s \n", action);

  /* read action */
  if( strcmp(action, "off" ) == 0 ) {
          sprintf( cmd , "%s dpms force off; echo $?", DPMS_CONTROLLER);
  }
  else if( strcmp(action, "on" ) == 0 ) {
          sprintf( cmd , "%s dpms force on; echo $?", DPMS_CONTROLLER);
  }
  else {
        /* show status */
       sprintf( cmd , "%s q| awk '/Monitor/ {print $3}'", DPMS_CONTROLLER);
  }

  dbgtcos("tcosxmlrpc::tcos_dpms() cmd=\"%s\"\n", cmd);

  /* put error in line */
  strncpy(line, DPMS_CMD_ERROR, BIG_BUFFER);

  fp=(FILE*)popen(cmd, "r");
  fret = fgets( line, sizeof line, fp);
  remove_line_break(line);
  pclose(fp);
  
  
  dbgtcos("tcosxmlrpc::tcos_dpms() line=\"%s\"\n", line);

  return xmlrpc_build_value(env, "s", line );
}


