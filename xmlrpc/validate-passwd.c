/*
* validate-passwd.c part of tcosxmlrpc
*   => validate throught /etc/passwd
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


#include <pwd.h>
#include <sys/types.h>

#include "validate.h"
#include "debug.c"


char *get_passwd( char *username )
{
 struct passwd *pwd;

 dbgtcos("get_passwd() getting passwd from user=%s\n", username);

 pwd=getpwnam(username);
 if (pwd == NULL) {
   fprintf(stderr, "error get_passwd(): user not found.\n");
   return "";
   }
 else if ( strcmp(pwd->pw_passwd, "x") == 0 ) {
   fprintf(stderr, "info get_passwd(): shadow passwords.\n");
   return pwd->pw_passwd;
 }
 else
  return (pwd->pw_passwd);
}




char *validate_passwd(char *user, char *pass)
{
  char *cryptpass;

  cryptpass=get_passwd(user);

#ifdef VISIBLE_PASSWD
  dbgtcos( "info validate_passwd(): pass=%s crypt=%s.\n", crypt (pass, PASS_ID), cryptpass);
#endif

  if ( strcmp(crypt (pass, PASS_ID), cryptpass) == 0 )
     return LOGIN_OK;

  return LOGIN_NOPASS;
}



#ifndef HAVE_MAIN
int main() {
  char *username;
  username = "root";
  printf("user=%s pass=%s\n", username, get_passwd(username) );

  username = "mario";
  printf("user=%s pass=%s\n", username, get_passwd(username) );

  username = "pepe";
  printf("user=%s pass=%s\n", username, get_passwd(username) );
  return 0;
}
#endif

