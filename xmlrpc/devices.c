/*# devices.c return all block devices and set/get actions 2006-09-09 14:22:40 mariodebian $
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

#include "devices.h"
#include "xauth.h"

static xmlrpc_value *
tcos_devices(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {
  FILE *fp;
  char line[BSIZE];
  char *option;
  char *cmdline;
  char mycmd[BSIZE];
  char *cookie;
  char *hostname;
  int xauth_ok;

  /*char *login_ok;*/

  dbgtcos("tcosxmlrpc::tcos_devices() Init \n");

  /* read what option and cmdline params need */
  xmlrpc_parse_value(env, in, "(ssss)", &option, &cmdline, &cookie, &hostname);
  if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   dbgtcos("tcosxmlrpc::tcos_devices() option=%s cmdline=%s \n", option, cmdline);

  if (strcmp(option, "--getxdrivers") == 0 ) {
     dbgtcos("tcosxmlrpc::tcos_devices() --getxdrivers no xauth\n");
  }
  else {  
    /* need XAUTH first */
    xauth_ok=handle_xauth(cookie,hostname);
    if( xauth_ok != XAUTH_OK )
      return xmlrpc_build_value(env, "s", "error: xauth access denied" );
  }

   dbgtcos("tcosxmlrpc::tcos_devices() exec=\"%s %s %s\"\n", DEVICES_WRAPPER, option, cmdline);

   snprintf( (char*) mycmd, BSIZE, "%s %s %s", DEVICES_WRAPPER, option, cmdline);

   fp=(FILE*)popen( mycmd , "r");
   if (fp == NULL)
      return xmlrpc_build_value(env, "s", DEVICES_READING_ERROR );

   /* put error msg into line var */
   strncpy(line, DEVICES_ERROR, BSIZE);

   fgets( line, sizeof line, fp);
   if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", DEVICES_READING_ERROR);

   pclose(fp);
   return xmlrpc_build_value(env, "s", line );


  /* never here */
  return xmlrpc_build_value(env, "s", DEVICES_UNKNOW_ERROR );
}




