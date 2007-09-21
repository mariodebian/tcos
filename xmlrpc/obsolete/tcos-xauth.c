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

#define OK 0
#define ERROR 1
#define FAIL -1


extern int noauth;
extern int authenticated;



void
handle_xauth( char *cookie , char *servername)
{
  /*int authfd;*/
  char hostname[BUFSIZ];
  char displayname[BUFSIZ];
  /*char *auth_file;*/
  Display* displ;
  /*size_t auth_size;*/
  int found = 0, i;
/*  char cmd[BUFSIZ];*/
  char cmd[BUFSIZ];
  /*char *cmd[] = { "xauth -f /tmp/.tmpxauth", " add ", "", ":0 MIT-MAGIC-COOKIE-1 ", ""  };*/

  printf( "handle_auth() cookie=%s server=%s\n" ,cookie, servername );


    if ( strcmp (servername, "") == 0 )
       gethostname(hostname, BUFSIZ);                      /* get our hostname */
    else
       sprintf(hostname, "%s" ,servername);


    printf("hostname=%s\n", hostname);
    printf("cookie=%s\n",   cookie);


    sprintf ( (char*) cmd, "xauth -f /tmp/.tmpxauth add %s:0 MIT-MAGIC-COOKIE-1 %s", hostname, cookie);

    printf( "cmd=\"%s\"\n", cmd );

    unlink("/tmp/.tmpxauth");                              /* delete file */
    system(cmd);


    setenv("XAUTHORITY", "/tmp/.tmpxauth", 1);          /* for XOpenDisplay */
    printf ( "XAUTHORITY = %s \n", getenv("XAUTHORITY")  );

    for (i = 0; i < 1; i++) {
      sprintf(displayname, "%s:%d", hostname, i);               /* displayify it */

      printf( "trying to connect to %s\n", displayname );

      displ = XOpenDisplay(displayname);

      if (displ) {
        found++;
        XCloseDisplay(displ);                           /* close display */
        break;
      }
    } /* end of for */

    /*unlink(XauFileName()); */                              /* delete file */

    if (!found) {
      errno = EACCES;
      /*status_return(sockfd, FAIL);*/
      printf ( "error openning DISPLAY \n" );
      exit(ERROR);
    }
  

  /*status_return(sockfd, OK);*/                            /* Acknowledge auth */
  printf ( "AUTH ok.\n" );
  /*authenticated++;*/                                      /* Set auth state */
}

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
