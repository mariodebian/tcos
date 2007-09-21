/*# standalone.c methods that get standalone info 2006-09-09 14:22:40 mariodebian $
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

#include "standalone.h"


static xmlrpc_value *
tcos_standalone(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {
  FILE *fp;
  char line[BIG_BUFFER];
  char *info;
  size_t *len;

  /* read what info search */
  xmlrpc_parse_value(env, in, "(s#)", &info, &len);

  dbgtcos("tcosxmlrpc::tcos_standalone() searching for standalone=\"%s\"\n", info);

  if ( strcmp(info, "get_user") == 0 )
      fp=(FILE*)popen(STANDALONE_USER, "r");

  else if ( strcmp(info, "get_process") == 0 )
      fp=(FILE*)popen(STANDALONE_PROCESS, "r");

  else if ( strcmp(info, "get_server") == 0 )
      fp=(FILE*)popen(STANDALONE_SERVER, "r");

  /* default method = error */
  else
      return xmlrpc_build_value(env, "s", STANDALONE_UNKNOW );

  if (fp == NULL)
	return xmlrpc_build_value(env, "s", STANDALONE_UNKNOW );

  /* put error into line */
  strncpy(line, STANDALONE_ERROR, BSIZE);

  fgets( line, sizeof line, fp);

  dbgtcos("tcosxmlrpc::tcos_standalone() line=\"%s\"\n", line);

  pclose(fp);
  return xmlrpc_build_value(env, "s", line );  
}


static xmlrpc_value *
tcos_dbus(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {
  FILE *fp;
  char cmd[BIG_BUFFER];
  char line[BIG_BUFFER];
  char *dbus;

  char *user;
  char *pass;
  char *login_ok;

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
  strncpy(line, DBUS_ERROR, BSIZE);

  fgets( line, sizeof line, fp);

  pclose(fp);
  return xmlrpc_build_value(env, "s", line );  
}




