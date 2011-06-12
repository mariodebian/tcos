/*
* config.c part of tcosxmlrpc
*   => get TCOS config vars
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
#include "xauth.h"
#include "config.h"


#if NEWAPI
xmlrpc_value *tcos_config(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_config(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char line[BSIZE];
  char *option;
  char *cmdline;
  char mycmd[BSIZE];
  char *user;
  char *pass;
  char *login_ok;
  char hostname[BSIZE];
  int xauth_ok;
  struct ip_address ip;
  char ip_string[BSIZE];
  /*char *fret;*/

  dbgtcos("tcosxmlrpc::tcos_config() Init \n");

  /* read what option and cmdline params need */
  xmlrpc_parse_value(env, in, "(ssss)", &option, &cmdline, &user, &pass);
  if (env->fault_occurred)
    return xmlrpc_build_value(env, "s", "params error");

  gethostname(hostname,BSIZE);
  fp=(FILE*)popen(MY_IP_ADDRESS, "r");
  (void)fgets( ip_string, sizeof ip_string, fp);
  remove_line_break(ip_string);
  pclose(fp);

  ip=check_ip_address(ip_string);

  dbgtcos("tcosxmlrpc::tcos_config() option=%s cmdline=%s user=%s pass=**notshow**\n", option, cmdline, user);

  if ( (strcmp(pass, hostname ) == 0) || (strcmp(pass, ip.ipstr) == 0) ) {
    /* need XAUTH first */
    xauth_ok=handle_xauth(user,pass);
    if( xauth_ok != XAUTH_OK )
      return xmlrpc_build_value(env, "s", "error: xauth access denied" );
  }
  else {
    /* need login first */
    login_ok=validate_login(user,pass);
    if( strcmp(login_ok,  LOGIN_OK ) != 0 )
      return xmlrpc_build_value(env, "s", login_ok );
  }


   dbgtcos("tcosxmlrpc::tcos_config() exec=\"%s %s %s\"\n", CONFIG_WRAPPER, option, cmdline);

   snprintf( (char*) mycmd, BSIZE, "%s %s %s", CONFIG_WRAPPER, option, cmdline);

   fp=(FILE*)popen( mycmd , "r");
   if (fp == NULL)
     return xmlrpc_build_value(env, "s", CONFIG_READING_ERROR );

   /* put -1 into line var */
   strncpy(line, CONFIG_ERROR, BSIZE);

   (void)fgets( line, sizeof line, fp);
   remove_line_break(line);
   pclose(fp);

   if (env->fault_occurred) {
     return xmlrpc_build_value(env, "s", CONFIG_READING_ERROR);
   }

   return xmlrpc_build_value(env, "s", line );
}



