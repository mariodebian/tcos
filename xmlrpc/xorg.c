/*xorg.c througt configurexorg script make some actions and get info
#  2006-09-09 14:22:40 mariodebian $
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

#include "xorg.h"



static xmlrpc_value *
tcos_xorg(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {
  FILE *fp;
  char line[BSIZE];
  char *option;
  char *cmdline;
  char *user;
  char *pass;
  char *login_ok;
  

  /* read what option and cmdline params need */
  xmlrpc_parse_value(env, in, "(ssss)", &option, &cmdline, &user, &pass);
  if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  dbgtcos("tcosxmlrpc::tcos_xorg() option=\"%s\" cmdline=\"%s\"\n", option, cmdline);

  /* generate new xorg.conf */
  if ( strcmp(option, "new") == 0 )
  {
   snprintf( (char*) line, BSIZE, "%s %s", XORG_NEW_CONF, cmdline );

   dbgtcos("tcosxmlrpc::tcos_xorg() new exec=\"%s\"\n", line);

   if (system(line) != 0 )
      return xmlrpc_build_value(env, "s", XORG_ERROR );
   else
      return xmlrpc_build_value(env, "s", XORG_OK );
  }

  /* change xorg.conf */
  else if ( strcmp(option, "change") == 0 )
  {
   snprintf( (char*) line, BSIZE, "%s %s", XORG_CHANGE_CONF, cmdline );

   dbgtcos("tcosxmlrpc::tcos_xorg() change exec=\"%s\"\n", line);

   if (system(line) != 0 )
      return xmlrpc_build_value(env, "s", XORG_ERROR );
   else
      return xmlrpc_build_value(env, "s", XORG_OK );
  }

  /* rebuild xorg.conf */
  else if ( strcmp(option, "rebuild") == 0 )
  {
   snprintf( (char*) line, BSIZE, "%s %s", XORG_REBUILD_CONF, cmdline );

   dbgtcos("tcosxmlrpc::tcos_xorg() rebuild exec=\"%s\"\n", line);

   if (system(line) != 0 )
      return xmlrpc_build_value(env, "s", XORG_ERROR );
   else
      return xmlrpc_build_value(env, "s", XORG_OK );
  }
 
  else if ( strcmp(option, "get") == 0 )
  {

   dbgtcos("tcosxmlrpc::tcos_xorg() get xorg.conf exec=\"%s\"\n", XORG_GET_CONF);

   fp=(FILE*)popen(XORG_GET_CONF, "r");
   if (fp == NULL)
      return xmlrpc_build_value(env, "s", XORG_READING_ERROR );

   /* put error msg into line var */
   strncpy(line, XORG_ERROR, BSIZE);

   fgets( line, sizeof line, fp);
   if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", XORG_READING_ERROR);

   pclose(fp);
   return xmlrpc_build_value(env, "s", line );  
  }


  else {
	return xmlrpc_build_value(env, "s", XORG_UNKNOW_OPTION );
  }




  /* never here */
  return xmlrpc_build_value(env, "s", XORG_UNKNOW_OPTION );
}




