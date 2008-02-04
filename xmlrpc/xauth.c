#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/Xauth.h>

#include "xauth.h"




struct ip_address check_ip_address(char *data) {
  struct ip_address ip;
  /*
  *  split ip address with 4 dots
  *  join ip address in ip.ipstr
  *  and compare with original data
  *
  */
  sscanf(data, "%d.%d.%d.%d", &ip.data[0], &ip.data[1], &ip.data[2], &ip.data[3]);

  snprintf(ip.ipstr, IP_LENGHT, "%d.%d.%d.%d", ip.data[0], ip.data[1], ip.data[2], ip.data[3]);

  dbgtcos("tcosxmlrpc::is_ip() comparing %s with %s ...\n", data, ip.ipstr);

  if ( strcmp(data, ip.ipstr) == 0 ) { 
    /*dbgtcos("tcosxmlrpc::is_ip() True ...\n");*/
    ip.is_ip=1;
  }
  else {
    /*dbgtcos("tcosxmlrpc::is_ip() ip.data[0]=%d\n", ip.data[0]);
    dbgtcos("tcosxmlrpc::is_ip() ip.data[1]=%d\n", ip.data[1]);
    dbgtcos("tcosxmlrpc::is_ip() ip.data[2]=%d\n", ip.data[2]);
    dbgtcos("tcosxmlrpc::is_ip() ip.data[3]=%d\n", ip.data[3]);
    dbgtcos("tcosxmlrpc::is_ip() False ...\n");*/
    ip.is_ip=0;
  }
  return ip;
}



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
    FILE *fp;

    dbgtcos("tcosxmlrpc::handle_auth() cookie=%s server=%s\n" ,cookie, servername);

    /* read my hostname */
    gethostname(host, BSIZE);
    /*dbgtcos("tcosxmlrpc::handle_xauth() gethostname=%s\n", host);*/

    /* check if servername is an IP address (see xauth.h for struct) */
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


    mkstemp(xauth_file);

    /*dbgtcos("tcosxmlrpc::handle_xauth() xauth_file=%s\n", xauth_file);*/

    snprintf ( (char*) cmd, BSIZE, "xauth -q -f %s add %s:0 MIT-MAGIC-COOKIE-1 %s >/dev/null 2>&1", xauth_file, host, cookie);

    dbgtcos("tcosxmlrpc::handle_xauth() cmd=\"%s\"\n", cmd);

    unlink(xauth_file);
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

    unlink(xauth_file);					/* delete XAUTHORITY temp file */
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




