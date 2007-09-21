/*validate-passwd.c validate throught /etc/passwd  2006-09-09 14:22:40 mariodebian $
#
# This file is part of tcosxmlrpc.
#
# tcosxmlrpc is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# tcosxmlrpc is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with tcosxmlrpc; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.
*/

#include <stdio.h>
#include <string.h>

#include <pwd.h>
#include <sys/types.h>

#include "validate.h"
#include "debug.c"


char *get_passwd( char *username )
{
 struct passwd *pwd;
 /*char *criptpass;*/

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
  /*struct passwd *pwd;*/
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

