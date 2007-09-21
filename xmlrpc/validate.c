/* 
 * validate.c
 *
 * Validate a username/pw combination, using the /etc/shadow
 * file.
 *
 * Two lines are read from stdin.  The first is the user, and 
 * the second is the (unencrypted) password.
 *
 * We exit with 0 if they match, 1 otherwise.
 * Errors are written to either stderr or the error log, or both.
 */

/*# validate.c LOGIN HELPER 2006-09-09 14:22:40 mariodebian $
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
#include <shadow.h>
#include <string.h>
#include <time.h>

#include "validate.h"

#include "validate-passwd.c"
#include "validate-shadow.c"
#include "validate-tcos.c"


int
check_for_file(char *fname)
{
/* return 0 if not found or cant read or 1 if found */

 FILE *fptemp;
 fptemp = fopen(fname,"r");
 if (!fptemp) {
    dbgtcos("info check_for_file(): File \"%s\" not found or cant read.\n", fname);
    return 0;
 }
 else
    dbgtcos("info check_for_file(): File \"%s\" found.\n", fname);
   return 1;
}


char
*validate_login(char *user, char*pw)
{
/* check for files in order */
   if ( check_for_file("/etc/tcospasswd") == 1 ) {
     dbgtcos("info validate_login(): /etc/tcospasswd exists.\n");
     return validate_tcos(user, pw);
   }
   else if ( check_for_file("/etc/shadow") == 1 ) {
     dbgtcos("info validate_login(): /etc/shadow exists.\n");
     return validate_shadow(user, pw);
   }
   else if ( check_for_file("/etc/passwd") == 1 ) {
     dbgtcos("info validate_login(): /etc/passwd exists.\n");
     return validate_passwd(user, pw);
   }
   else {
     dbgtcos("error validate_login(): no files found.\n");
     return (char*) LOGIN_ERROR;
   }

  return (char*) LOGIN_ERROR;
}


char 
*validate_login2(char *user, char *pw)
{

    /* check for /etc/shadow */
    struct passwd *pwd;
    
    pwd=getpwnam(user);
    if (pwd == NULL) {
      fprintf(stderr, "error get_passwd(): user not found.\n");
      return LOGIN_NOUSER;
    }
    else if ( strcmp(pwd->pw_passwd, "x") == 0 ) {
      fprintf(stderr, "info get_passwd(): shadow passwords.\n");
      /* try /etc/shadow */
      
      return validate_shadow(user, pw);
      
    }
    
    else {
       fprintf(stderr, "info get_passwd(): no shadow passwords.\n");
       /* try /etc/passwd */
       
       return validate_passwd(user, pw);
    }
    
    return((char*) LOGIN_ERROR);    
}


