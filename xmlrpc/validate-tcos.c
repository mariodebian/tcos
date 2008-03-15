/*
* validate-tcos.c part of tcosxmlrpc
*   => validate throught /etc/tcospasswd
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


#define _GNU_SOURCE
#include <string.h>

#include <pwd.h>
#include <sys/types.h>

#include "validate.h"


void
split_login( struct info *login )
{
int i;
char *p;
  p = strtok (login->line,":");
  i=0;
  while (p != NULL)
  {
    if(i==0){
      login->user=p;
      i++;
    }
    else if(i==1){
      login->pass=p;
      i++;
    }
    p = strtok (NULL, "");
  }
 /* clean last char of password */
 login->pass[strlen(login->pass)-1]='\0';
}




char *validate_tcos(char *user, char *pass)
{
  /*struct passwd *pwd;*/
  char *cryptpass;
  FILE *fp;
  char line[BSIZE];
  struct info *login=malloc(sizeof(struct info));
  fp = fopen ("/etc/tcospasswd", "r" );
  if (fp == NULL) {
     dbgtcos("error validate_tcos(): file /etc/tcospasswd not exists.\n");
     return (char*) LOGIN_ERROR;
  }

  fgets( line, sizeof line, fp);
  strncpy(login->line, line, BSIZE);
  fclose(fp);
  split_login(login);

  #ifdef VISIBLE_PASSWD 
    dbgtcos( "validate_tcos() user=\"%s\" pass=\"%s\"\n", login->user, login->pass);
  #endif

  dbgtcos( "validate_tcos() check users user=\"%s\" my_user=\"%s\"\n", user, login->user);

  if ( strcmp(login->user, user) != 0 ) {
     dbgtcos("error validate_passwd(): BAD USER.\n");  
     return LOGIN_NOUSER;
  }

  cryptpass=crypt(pass, PASS_ID );

  #ifdef VISIBLE_PASSWD 
    dbgtcos("info validate_passwd(): the_pass=%s my_pass=%s \n", login->pass, cryptpass);
  #endif

  if ( strcmp(login->pass, cryptpass) == 0 ) {
     dbgtcos("info validate_passwd(): LOGIN OK.\n");
     free(login);
     return LOGIN_OK;
  }
  dbgtcos("info validate_passwd(): BAD PASSWORD.\n");
  free(login);
  return LOGIN_NOPASS;
}



#ifndef HAVE_MAIN
int main(int argc, char **argv) {
  char *username;
  char *password;
  if (argc != 3) {
   printf("Need to put user and pass params.\n");
   printf("Example:  TCOS_DEBUG=1 ./validate-tcos user pass\n");
   return 1;
  }
  username = argv[1];
  password = argv[2];
  printf("main() user=%s pass=%s\n", username, password );
  printf("main() ?= %s.\n", validate_tcos(username, password) );

  return 0;
}
#endif



