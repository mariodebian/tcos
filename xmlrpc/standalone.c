/*
* standalone.c part of tcosxmlrpc
*   => methods that get standalone info
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
#include "standalone.h"

#if NEWAPI
xmlrpc_value *tcos_standalone(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_standalone(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char line[BIG_BUFFER];
  char *info;
  char *group;
  size_t *len;
  char *fret;

  /* read what info search */
  xmlrpc_parse_value(env, in, "(ss#)", &info, &group, &len);

  dbgtcos("tcosxmlrpc::tcos_standalone() searching for standalone=\"%s\"\n", info);

  if ( strcmp(info, "get_user") == 0 )
    fp=(FILE*)popen(STANDALONE_USER, "r");

  else if ( strcmp(info, "get_exclude") == 0 ) {
    snprintf( line, BSIZE, "%s=%s 2>/dev/null", STANDALONE_EXCLUDE, group);
    fp=(FILE*)popen(line, "r");
  }
  else if ( strcmp(info, "get_process") == 0 )
    fp=(FILE*)popen(STANDALONE_PROCESS, "r");

  else if ( strcmp(info, "get_server") == 0 ) {
    snprintf( line, BSIZE, "%s %s 2>/dev/null", STANDALONE_SERVER, group);
    fp=(FILE*)popen(line, "r");
  }
  else if ( strcmp(info, "get_time") == 0 )
    fp=(FILE*)popen(STANDALONE_TIME, "r");

  /* default method = error */
  else
    return xmlrpc_build_value(env, "s", STANDALONE_UNKNOW );

  if (fp == NULL)
    return xmlrpc_build_value(env, "s", STANDALONE_UNKNOW );

  /* put error into line */
  strncpy(line, STANDALONE_ERROR, BIG_BUFFER);

  fret = fgets( line, sizeof line, fp);
  remove_line_break(line);
  pclose(fp);

  dbgtcos("tcosxmlrpc::tcos_standalone() line=\"%s\"\n", line);
  return xmlrpc_build_value(env, "s", line );
}


#if NEWAPI
xmlrpc_value *tcos_dbus(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_dbus(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char cmd[BIG_BUFFER];
  char line[BIG_BUFFER];
  char *dbus;

  char *user;
  char *pass;
  char *login_ok;
  char *fret;

  /* read what dbus search */
  xmlrpc_parse_value(env, in, "(sss)", &dbus, &user, &pass);

  /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );


  dbgtcos("tcosxmlrpc::tcos_dbus() searching for dbus=\"%s\"\n", dbus);

  snprintf( cmd, BSIZE, "%s %s", DBUS_HANDLER, dbus);

  dbgtcos("tcosxmlrpc::tcosdbus() exe=\"%s\"\n", cmd);

  fp=(FILE*)popen(cmd, "r");

  if (fp == NULL)
    return xmlrpc_build_value(env, "s", DBUS_ERROR );

  /* put error into line var */
  strncpy(line, DBUS_ERROR, BIG_BUFFER);

  fret = fgets( line, sizeof line, fp);
  remove_line_break(line);
  pclose(fp);

  return xmlrpc_build_value(env, "s", line );
}


