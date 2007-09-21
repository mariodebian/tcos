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

int snprintf(char *str, size_t size, const char *format, ...);

int
handle_xauth( char *cookie , char *servername)
{
  char hostname[BSIZE];
  char displayname[BSIZE];
  Display* displ;
  int found = 0, i;
  char cmd[BSIZE];

  dbgtcos("tcosxmlrpc::handle_auth() cookie=%s server=%s\n" ,cookie, servername);

    /*
    if ( strcmp (servername, "") == 0 )
       gethostname(hostname, BSIZE);
    else
       snprintf(hostname, BSIZE, "%s" ,servername);
    */

    /* read my hostname */
    gethostname(hostname, BSIZE);
    dbgtcos("tcosxmlrpc::handle_xauth() gethostname=%s\n", hostname);


    /* compare with cookie hostname */
    if (strcmp (servername, hostname) != 0 ) {
       dbgtcos("tcosxmlrpc::handle_xauth() ERROR servername != hostname");
       return(XAUTH_ERROR);
    }

    snprintf ( (char*) cmd, BSIZE, "xauth -q -f /tmp/.tmpxauth add %s:0 MIT-MAGIC-COOKIE-1 %s", hostname, cookie);

    dbgtcos("tcosxmlrpc::handle_xauth() cmd=\"%s\"\n", cmd);

    unlink("/tmp/.tmpxauth");
    system(cmd);

    setenv("XAUTHORITY", "/tmp/.tmpxauth", 1);          /* for XOpenDisplay */

    dbgtcos("tcosxmlrpc::handle_xauth() XAUTHORITY=%s \n", getenv("XAUTHORITY"));


    for (i = 0; i < 1; i++) {
      snprintf(displayname, BSIZE, "%s:%d", hostname, i);               /* displayify it */

      dbgtcos("tcosxmlrpc::handle_xauth() trying to connect to %s\n", displayname);

      displ = XOpenDisplay(displayname);

      if (displ) {
        found++;
        XCloseDisplay(displ);                           /* close display */
        break;
      }
    } /* end of for */

    unlink(XauFileName());                              /* delete file */

    if (!found) {
      dbgtcos("error openning DISPLAY \n");
      return(XAUTH_ERROR);
    }

    dbgtcos("tcosxmlrpc::handle_xauth() AUTH ok.\n");  

  return(XAUTH_OK);
}



static xmlrpc_value *
tcos_xauth(xmlrpc_env *env, xmlrpc_value *in, void *ud)
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

  return xmlrpc_build_value(env, "s", "xauth: access OK " );
}






/*
int main( int argc, char **argv ) {
  if(argc != 3) {
    printf( "Need 2 arguments, first cookie, last hostname to connect.\n" );
    return(1);
  }
  printf ( "main() argv=%s\n" , argv[1] );
  printf( "handle_auth()  cookie=%s hostname=%s\n" , argv[1] , argv[2] );
  handle_xauth( argv[1] , argv[2] );
  return(0);
}
*/
