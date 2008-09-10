/*
* tcos-net-controller.c
*   => iptables controller
* Copyright (C) 2006,2007,2008  mariodebian at gmail
* Copyright (C) 2006,2007,2008  vidal_joshur at gva.es
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

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <stdarg.h>

#include <net/if.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/route.h>

#define GROUP "tcos"
#define IPTABLES "/sbin/iptables"
#define BLACKLIST_PORTS "8998,8999"
#define BSIZE 1024

#ifndef DEVNULL
#define DEVNULL "2>/dev/null"
#endif

void create_route(struct rtentry *p, char **args);

char *strncpy( char *to, const char *from, size_t count );

FILE *popen(const char *orden, const char *tipo);
int pclose(FILE *flujo);

void *malloc(size_t size);

int getgroups(int size, gid_t list[]);

/* int strcmp(const char *str1, const char *str2); */
char *strtok( char *str1, const char *str2 );

#define MAXTOKENS       256
#define MAXLINE         1024     /* fgets buff */
#define MINLEN          3        /* skip lines shorter as */

/* split string into tokens, return token array */
/* http://www.c.happycodings.com/Miscellaneous/code37.html */
char **split(char *string, char *delim) {
 char **tokens = NULL;
 char *working = NULL;
 char *token = NULL;
 int idx = 0;

 tokens  = malloc(sizeof(char *) * MAXTOKENS);
 if(tokens == NULL)
  return NULL;
 working = malloc(sizeof(char) * strlen(string) + 1);
 if(working == NULL)
  return NULL;

 /* to make sure, copy string to a safe place */
 strcpy(working, string);
 for(idx = 0; idx < MAXTOKENS; idx++)
  tokens[idx] = NULL;

 token = strtok(working, delim);
 idx = 0;

 /* always keep the last entry NULL termindated */
 while((idx < (MAXTOKENS - 1)) && (token != NULL)) {
  tokens[idx] = malloc(sizeof(char) * strlen(token) + 1);
  if(tokens[idx] != NULL) {
   strcpy(tokens[idx], token);
   idx++;
   token = strtok(NULL, delim);
  }
 }

 free(working);
 return tokens;
}

#ifdef DEBUG
/* debug print to stderr */
void debug( const char *format_str, ... ) {
    va_list ap;
    va_start( ap, format_str );
    va_end( ap );
    fprintf(stderr, "TNC-DEBUG::");
    vfprintf(stderr, format_str , ap);
}
#endif


int
check_port (char* port)
{
  char blacklist[] = BLACKLIST_PORTS;
  char *delim = ",";
  char **tokens = NULL;
  int i=0,j;

  if (atoi(port) > 65535 || atoi(port) < 1) {
#ifdef DEBUG
     debug("check_port() Incorrect port: %s\n", port);
#endif
     i=1;
     return(i);
  }

  tokens = split(blacklist, delim);
  for(j = 0; tokens[j] != NULL; j++){
#ifdef DEBUG
      debug("check_port(): token=%s ¿=? port=%s\n", tokens[j], port);
#endif
      if (atoi(port) == atoi(tokens[j])) {
          i=1;
      }
  }
  for(j = 0; tokens[j] != NULL; j++) {
    free(tokens[j]);
  }
  free(tokens);
  
  return i;
}


static unsigned int
get_uid (const char* user)
{
  struct passwd *pw = NULL;
  uid_t value=0;

  pw = getpwnam(user);

  if (pw == NULL)
    return value;
  else
    return pw->pw_uid;
}


static char
*get_group (gid_t gid)
{
  struct group *grp = NULL;

  grp = getgrgid (gid);

  if (grp == NULL)
    return "";
  else
    return grp->gr_name;
}


int
check_user ()
{
  gid_t *list;
  int i,n,found=0;
  uid_t uid, euid;
  
  if ((n = getgroups(0, NULL)) < 0) {
    found=0;
  }

  list = (gid_t*)malloc(sizeof (gid_t) * (n + 1));
  if (!list) {
#ifdef DEBUG
    debug("check_user() out of memory!\n");
#endif
    found=0;
  }

  n = getgroups(n, list);

  uid = getuid();
  euid = geteuid();

  if (uid == 0 && euid == 0) {
      found=1;
  }
  else {
     for (i = 0; i < n; i++)  {
#ifdef DEBUG
          debug("check_user() %lu (%s) %d %d\n", (unsigned long)list[i], get_group(list[i]), uid, euid);
#endif
          if ( (strcmp( get_group(list[i]), GROUP) == 0) && (euid == 0) ) {
            /* Only work if euid is root */
            found=1;
         }
    }
  }
  free(list);
  return(found);
}


int
status_iptables_user(char *args) {

   FILE *fp;
   char cmd[BSIZE];
   char line[BSIZE];
   char *fret;

    sprintf( cmd, "iptables -L OUTPUT --line-numbers -n %s | awk 'BEGIN{count=0}{if ($NF == %d || $NF == \"%s\") count++}END{print count}'", DEVNULL, (int)get_uid(args), args);
#ifdef DEBUG
    debug("status_iptables_user() %s\n",cmd); 
#endif
    if ((fp=(FILE*)popen(cmd, "r")) != NULL) {
       fret = fgets( line, sizeof line, fp);
       pclose(fp);
       if ( line[strlen(line)-1] == '\n' ) 
         line[strlen(line)-1] = 0;
       if ( strcmp( line, "0") != 0 )
         return(1);
    }
    return(0);
}


int
flush_iptables_user(char *args) {

   FILE *fp;
   char cmd[BSIZE];
   char line[BSIZE];
   int i;
   char *fret;

   char *delim = " ";
   char **tokens = NULL;

    if ( (i = status_iptables_user(args)) == 0)
       return(1);
   
    sprintf( cmd, "iptables -L OUTPUT --line-numbers -n %s | awk '{if ($NF == %d || $NF == \"%s\") print $1}' | tac | tr \"\\n\" \" \"", DEVNULL, (int)get_uid(args), args);
    if ((fp=(FILE*)popen(cmd, "r")) != NULL) {
       fret = fgets( line, sizeof line, fp);
       pclose(fp);
       if( line[strlen(line)-1] == '\n' ) 
         line[strlen(line)-1] = 0;

       tokens = split(line, delim);
       for(i = 0; tokens[i] != NULL; i++){
          sprintf( cmd, "%s -D OUTPUT %s %s", IPTABLES, tokens[i], DEVNULL);
#ifdef DEBUG
          debug("flush_iptables_user() %s\n",cmd);
#endif
          if ((fp=(FILE*)popen(cmd, "r")) != NULL) 
             pclose(fp);
       }
       for(i = 0; tokens[i] != NULL; i++) {
         free(tokens[i]);
       }
       free(tokens);
       return(1);
    }
    return(0);
}


int
add_iptables_user(char **args) {

   FILE *fp;
   char cmd[BSIZE];
   char *substring;
   char dirred[BSIZE];
   int i=0,j;
   char *fret;

   char *delim = ",";
   char **tokens = NULL;

    /* Delete rules that already exists for user*/
    flush_iptables_user(args[4]);


    /* Obtain network destination */
    sprintf( cmd, "ip route %s | awk '{if ($3 == \"%s\") print $1}'", DEVNULL, args[3]);
#ifdef DEBUG
    debug("add_iptables_user() dired cmd=%s\n",cmd);
#endif
    if ((fp=(FILE*)popen(cmd, "r")) != NULL) {
       fret = fgets( dirred, sizeof dirred, fp);
       pclose(fp);
       if( dirred[strlen(dirred)-1] == '\n' ) 
          dirred[strlen(dirred)-1] = 0;
    }
    else {
       return(0);
    }

    /* block ports if especified*/
    if(strstr(args[2], "--ports=")) {
       substring = (char*) malloc(strlen(args[2]));
       strncpy(substring, args[2]+8, strlen(args[2]));
#ifdef DEBUG
       debug("add_iptables_user() substring='%s'\n",substring);
#endif
       
       tokens = split(substring, delim);
       for(j = 0; tokens[j] != NULL; j++){
            if (check_port(tokens[j]) == 0) {
               sprintf( cmd, "%s -A OUTPUT -p tcp --dport %s -m owner --uid-owner %s -j DROP %s", IPTABLES, tokens[j], args[4], DEVNULL);
#ifdef DEBUG
               debug("add_iptables_user() cmd=%s\n",cmd);
#endif
               if ((fp=(FILE*)popen(cmd, "r")) != NULL) 
                  pclose(fp);
            }
       }
       
       for(j = 0; tokens[j] != NULL; j++) {
         free(tokens[j]);
       }
       free(tokens);
       free(substring);
    }
    
    /* Allow loopback for user*/
    sprintf( cmd, "%s -A OUTPUT -d 127.0.0.0/255.0.0.0 -m owner --uid-owner %s -j ACCEPT %s", IPTABLES, args[4], DEVNULL);
#ifdef DEBUG
    debug("add_iptables_user() cmd=%s\n",cmd);
#endif
    if ((fp=(FILE*)popen(cmd, "r")) != NULL) {
      i=1;
      pclose(fp);
    }
    else {
      return(0);
    }
    
    /* Block output ! network*/
    sprintf( cmd, "%s -A OUTPUT -d ! %s -m owner --uid-owner %s -j DROP %s", IPTABLES,  dirred, args[4], DEVNULL);
#ifdef DEBUG
    debug("add_iptables_user() cmd=%s\n",cmd);
#endif
    if ((fp=(FILE*)popen(cmd, "r")) != NULL) {
      i=1;
      pclose(fp);
    }
    else {
      return(0);
    }

    return i;

}

char 
*ip_by_eth(char *dev) {

    struct sockaddr_in *sin;
    struct ifreq ifr;
    int fd; 


    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return "error";

    strcpy( ifr.ifr_name, dev );

    if (ioctl( fd, SIOCGIFADDR, &ifr ) == 0) { 
       sin = (struct sockaddr_in *)&ifr.ifr_addr;
       close( fd );
       return inet_ntoa( sin->sin_addr );
    }
    return "error";

}

int 
add_route( char **args ) {

    struct rtentry route;
    int i = 1;
    int fd;

    create_route(&route, args);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return 0;


    /* del the route if its set before */
    ioctl( fd, SIOCDELRT, &route );
    if( ioctl( fd, SIOCADDRT, &route ) < 0) {
        i = 0;
    }

    close( fd );
    return i;
}


int 
del_route(char **args) {

    struct rtentry route;
    int i = 1;
    int fd;

    create_route(&route, args);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return 0;


    /* del the route if its set before */
    if (ioctl( fd, SIOCDELRT, &route ) < 0) {
        i = 0;
    }

    close( fd );
    return i;
}


int main(int argc, char **argv) {
   int i=0;
   i = check_user();
   
   if (argc < 2) {
      return(1);
   }

   if (i == 0) {
     printf("denied");
     return(1);
   }


  if ( strcmp( argv[1], "disable-internet") == 0 && argc == 5) {
     i = add_iptables_user(argv);
     if (i == 0) {
        flush_iptables_user(argv[4]);
        printf("disable-error");
     }
     else {
        printf("disabled");
     }
  }
  else if ( strcmp( argv[1], "enable-internet") == 0 && argc == 3) {
     i = flush_iptables_user(argv[2]);
     if (i == 0) {
        printf("enable-error");
     }
     else {
        printf("enabled");
     }

  }
  else if ( strcmp( argv[1], "status") == 0 && argc == 3) {
     i = status_iptables_user(argv[2]);
     if (i == 0) {
        printf("enabled");
     }
     else {
        printf("disabled");
     }
  }
  else if ( strcmp( argv[1], "route-add") == 0 && argc == 5) {
     i = add_route(argv);
     if (i == 0) {
        printf("error");
     }
     else {
        printf("ok");
     }
  }
  else if ( strcmp( argv[1], "route-del") == 0 && argc == 5) {
     i = del_route(argv);
     if (i == 0) {
        printf("error");
     }
     else {
        printf("ok");
     }
  }
  else {
    fprintf(stderr, "ERROR => Bad command line arguments\n");
    fprintf(stderr, "tnc :: tcos-net-controller usage\n");
    fprintf(stderr, "\t tnc disable-internet --ports=[AA,BB,CC] ethX username\n");
    fprintf(stderr, "\t tnc enable-internet username\n");
    fprintf(stderr, "\t tnc route-add ip-multicast netmask ethX\n");
    fprintf(stderr, "\t tnc route-del ip-multicast netmask ethX\n");
    fprintf(stderr, "\t tnc status username\n");
    return(1);
  }

  return(0);
}


void create_route(struct rtentry *p, char **args) {

    struct sockaddr_in singw, sindst, sinmask;
    /*char mask[]="255.255.0.0";*/

    memset( &singw, 0, sizeof( struct sockaddr ) );
    memset( &sindst, 0, sizeof( struct sockaddr ) );
    memset( &sinmask, 0, sizeof( struct sockaddr ) );
    singw.sin_family = AF_INET;
    sindst.sin_family = AF_INET;
    sinmask.sin_family = AF_INET;

    sindst.sin_addr.s_addr = inet_addr( args[2] );
    singw.sin_addr.s_addr = inet_addr( ip_by_eth(args[4]) );
    sinmask.sin_addr.s_addr = inet_addr( args[3] );

    memset( p, 0, sizeof( struct rtentry ) );
    (*p).rt_dst = *(struct sockaddr *)&sindst;
    (*p).rt_gateway = *(struct sockaddr *)&singw;
    (*p).rt_genmask = *(struct sockaddr *)&sinmask;
    (*p).rt_flags = RTF_GATEWAY;

}

