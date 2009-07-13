/*
* xorg.c part of tcosxmlrpc
*   => througt configurexorg script make some actions and get info
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
#include "validate.h"
#include "debug.h"
#include "xorg.h"


#if NEWAPI
xmlrpc_value *tcos_xorg(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_xorg(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char line[BSIZE];
  char *option;
  char *cmdline;
  char *user;
  char *pass;
  char *login_ok;
  char *fret;


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

   fret = fgets( line, sizeof line, fp);
   remove_line_break(line);
   pclose(fp);

   if (env->fault_occurred) {
     return xmlrpc_build_value(env, "s", XORG_READING_ERROR);
   }

  return xmlrpc_build_value(env, "s", line );
  }


  else {
    return xmlrpc_build_value(env, "s", XORG_UNKNOW_OPTION );
  }

}



