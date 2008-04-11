/*
* reboot_poweroff.c part of tcosxmlrpc
*   => reboot or poweroff thin client using Xorg auth
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


#include "reboot_poweroff.h"


#if NEWAPI
static xmlrpc_value *tcos_reboot_poweroff(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
static xmlrpc_value *tcos_reboot_poweroff(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char *option;
  char *user;
  char *pass;
  char *login_ok;
  char hostname[BSIZE];
  int xauth_ok;
  struct ip_address ip;
  char ip_string[BSIZE];

  dbgtcos("tcosxmlrpc::tcos_reboot_poweroff() Init \n");

  /* read what option and cmdline params need */
  xmlrpc_parse_value(env, in, "(sss)", &option, &user, &pass);
  if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

  gethostname(hostname,BSIZE);
  fp=(FILE*)popen(MY_IP_ADDRESS, "r");
  fgets( ip_string, sizeof ip_string, fp);
  pclose(fp);

  ip=check_ip_address(ip_string);

  dbgtcos("tcosxmlrpc::tcos_reboot_poweroff() option=%s user=%s pass=%s\n", option, user, pass);

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


   dbgtcos("tcosxmlrpc::tcos_reboot_poweroff() exec=\"%s\"\n", option);

   if (strcmp(option, "reboot") == 0 )
       job_exe("reboot");

   else if (strcmp(option, "poweroff") == 0 )
       job_exe("poweroff");

   else
       return xmlrpc_build_value(env, "s", RP_UNKNOW );

  return xmlrpc_build_value(env, "s", RP_OK );
}



