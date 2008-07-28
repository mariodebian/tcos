/*
* xauth.c part of tcosxmlrpc
*   => validate with Xorg MIT MAGIC COOKIE
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



#include <X11/Xlib.h>
#include <X11/Xauth.h>

#include "xauth.h"





int
handle_xauth( char *cookie , char *servername)
{
    char host[BSIZE];
    char displayname[BSIZE];
    Display* displ;
    int found = 0;
    char cmd[BSIZE];
    struct ip_address ip;
    char xauth_file[]="/tmp/.tmpxauthXXXXXX";
    int fd;
    FILE *fp;

    dbgtcos("tcosxmlrpc::handle_auth() cookie=%s server=%s\n" ,cookie, servername);

    /* read my hostname */
    gethostname(host, BSIZE);
    /*dbgtcos("tcosxmlrpc::handle_xauth() gethostname=%s\n", host);*/

    /* check if servername is an IP address (see common.h for struct) */
    ip=check_ip_address(servername);

    /* compare with cookie hostname */
    if (ip.is_ip == 0) {
       if ( strcmp (servername, host) != 0 ) {
         dbgtcos("tcosxmlrpc::handle_xauth() ERROR servername != hostname\n");
         return(XAUTH_BAD_HOSTNAME);
       }
       dbgtcos("tcosxmlrpc::handle_xauth() not ip using hostname\n");
    }
    else {
        dbgtcos("tcosxmlrpc::handle_xauth() ip True\n");
        snprintf(host, BSIZE, "%s", ip.ipstr);
    }
    
    fd = mkstemp(xauth_file);
    if (fd < 0) {
        unlink(xauth_file);
        close(fd);
        return(XAUTH_BAD_FDTEMP);
    }

    /*dbgtcos("tcosxmlrpc::handle_xauth() xauth_file=%s\n", xauth_file);*/

    snprintf ( (char*) cmd, BSIZE, "xauth -q -f %s add %s:0 MIT-MAGIC-COOKIE-1 %s >/dev/null 2>&1", xauth_file, host, cookie);

    dbgtcos("tcosxmlrpc::handle_xauth() cmd=\"%s\"\n", cmd);

    /*unlink(xauth_file);*/
    fp=(FILE*)popen(cmd, "r");
    pclose(fp);

    setenv("XAUTHORITY", xauth_file, 1);          /* for XOpenDisplay */ 

    /*dbgtcos("tcosxmlrpc::handle_xauth() XAUTHORITY=%s \n", getenv("XAUTHORITY"));*/


    snprintf(displayname, BSIZE, "%s:0", host);               /* displayify it */
    displ = XOpenDisplay(displayname);

    if (displ) {
      found++;
      XCloseDisplay(displ);                           /* close display */
    }

    unlink(xauth_file); 				/* delete XAUTHORITY temp file */
    close(fd);
    unsetenv("XAUTHORITY");				/* unset environment XAUTH */

    if (!found) {
      dbgtcos("error openning DISPLAY to server %s\n", displayname);
      return(XAUTH_ERROR);
    }

    /*dbgtcos("tcosxmlrpc::handle_xauth() connect to %s AUTH ok.\n", displayname);*/

    return(XAUTH_OK);
}


#if NEWAPI
static xmlrpc_value *tcos_xauth(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
static xmlrpc_value *tcos_xauth(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  char *cookie;
  char *hostname;
  int xauth_ok;
  

  /* read what option and cmdline params need */
  xmlrpc_parse_value(env, in, "(ss)", &cookie, &hostname);
  if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   /* need login first */
  xauth_ok=handle_xauth(cookie,hostname);
  if( xauth_ok != XAUTH_OK )
    return xmlrpc_build_value(env, "s", "xauth: error access denied" );

  return xmlrpc_build_value(env, "s", "xauth: access OK" );
}




