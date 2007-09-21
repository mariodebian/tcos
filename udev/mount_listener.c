/*

  Usage:
     mount-listener /proc/mounts /tmp/proc_mounts

   listen in changes of /proc/mounts and return
   device mounted or umounted

  This is because in kernel >= 2.6.22 this feature is removed

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <poll.h>
#include <fcntl.h>

/* variable argument list */
#include <stdarg.h>

#define MSG_BUFF 4096
#define POLL_TIMEOUT 2*1000


int found_device=0;


void debug( const char *format_str, ... ) {
    va_list ap;
    va_start( ap, format_str );
    va_end( ap );
    vfprintf(stderr, format_str , ap);
}


int getnumlines( char *fname )
{
  FILE *fp;
  int lines=0;
  char line[MSG_BUFF];
  fp=fopen(fname, "r");
  if(!fp) return -1;
  while ( fgets(line, MSG_BUFF, fp) != NULL ) {
   ++lines;
  }
  fclose(fp);
  debug("DEBUG: getnumlines of '%s' %d\n", fname, lines);
  return lines;
}

void print_dev(char *txt, char *action)
{
  int i;
  for ( i=0; i<strlen(txt); i++) {
     if ( txt[i] == ' ') {
       printf(" %s\n", action);
       return;
     }
     printf("%c", txt[i]);
  }
}


void getlastlines(char *fname, int num, int total, char *action)
{
  FILE *fp;
  char line[MSG_BUFF];
  int i=0;
  fp=fopen(fname, "r");
  if(!fp) return;
  debug("DEBUG: getlastlines() total=%d num=%d\n", total, num);
  while ( fgets(line, MSG_BUFF, fp) != NULL ) {
    i++;
    if (i > total - num ) {
      if ( strncmp( line, "/dev", 4) != 0 ) {
        debug("DEBUG: getlastlines() device not start with /dev\n");
        continue;
      }

      if ( line[6] != 'd' ) {
        debug("DEBUG: getlastlines() device not have 'd' in /dev/?d* like /dev/sda1 for example\n");
        continue;
      }
        print_dev(line, action);
        found_device=1;
    }
  }
  fclose(fp);
  return;
}

void compare(char *fname1, char *fname2)
{
  int n1, n2;
  n1=n2=0;
  n1=getnumlines(fname1);
  n2=getnumlines(fname2);
  if (n1 == -1 || n2 == -1) {
   debug("DEBUG: error reading number of lines n1=%d, n2=%d\n", n1, n2);
   return;
  }

  if (n1 == n2) {
   debug("DEBUG: warning, files have the same number of files %d\n", n1);
   return;
  }

  if (n1 > n2) {
   /* mount */
   debug("DEBUG: compare MOUNT, diff=%d\n", n1-n2);
   getlastlines(fname1, n1-n2, n1, "mount");
  }

  if (n1 < n2) {
   /* umount */
   debug("DEBUG: compare UMOUNT, diff=%d\n", n2-n1);
   getlastlines(fname2, n2-n1, n2, "umount");
  }
}


int main (int argc, char *argv[]) {
  debug("DEBUG: *** comparing: %s <=> %s\n", argv[1], argv[2]);

  int fd_file;
  struct pollfd fdarray[1];
  int nfds, rc;

  if ((fd_file = open(argv[1], O_RDONLY, 0)) < 0) {
     perror("Error opening file ");
     return -1;
  }

  for (;;) {
    fdarray[0].fd = fd_file;
    fdarray[0].events = POLLIN;
    nfds = 1;

    rc = poll(fdarray, nfds, POLL_TIMEOUT);

    if (rc < 0) {
     perror("error reading poll() \n");
     return -1;
    }

    if(rc > 0) {
     compare(argv[1], argv[2]);
     if (found_device > 0 ) return 0;
    }
  }

  // never here
  return 0;
}



