/*login_old.h common headers  2006-09-09 14:22:40 mariodebian $
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

#define NO_ALLOWED "error: Access denied"

#define LOGGED_FILE "/tmp/logged"
#define PASSWD_FILE "/etc/tcospasswd"


#define BSIZE 500

/* is_logged messages */
#define IL_OK 1
#define IL_ERROR_REMOVE 2
#define IL_NOLOGGED 3


#define NO_USER "error: User not allowed"
#define NO_PASS "error: Password not valid"
#define NO_DATA "error: User or password not received"
#define LOGIN_OK "ok"
#define NO_PASS_FILE "error: NO PASSWD_FILE file found"

typedef struct info {
  char data[150];
  char *user; /* server user */
  char *pass; /* server pass */
  int logged;
} login2 ;


#include "md5.h"
#include "common.h"


int is_logged() {
 FILE *fp;
 /* fprintf(stderr, "::::opening file:::\n"); */
 fp=fopen(LOGGED_FILE, "r");
 
 /* if file exists */
 if (fp != NULL)
 {
 fclose(fp);
 /* fprintf(stderr, "::::pointer not null:::\n"); */
    if ( remove(LOGGED_FILE) == 0)
    {
      /* fprintf(stderr, "::::file deleted:::\n"); */
      return IL_OK;
    }
    else
    {
      /* fprintf(stderr, "::::error deleting file:::\n"); */
      return IL_ERROR_REMOVE;
    }
 }
 else
 {
  /* fprintf(stderr, "::::pointer null:::\n"); */
  return IL_NOLOGGED;
 } 
}





static xmlrpc_value *
tcos_login(xmlrpc_env *env, xmlrpc_value *in, void *ud)
 {
  FILE *fp;
  char *user;
  char *pass;
  
  struct info login, *plogin;
  plogin=&login;

 

  xmlrpc_parse_value(env, in, "(ss)", &user, &pass);

  fprintf(stderr, "tcosxmlrpc::tcos_login() user=\"%s\" pass=\"******\"\n", user);
  /* fprintf(stderr, "tcosxmlrpc::tcos_login() user=\"%s\" pass=\"%s\"\n", user, pass); */

  /*---------------------------------------------------------------*/
   /* open user:pass file */
 fprintf(stderr, "tcosxmlrpc::tcos_login() opening passwd file=\"%s\"\n", PASSWD_FILE);
 fp=fopen(PASSWD_FILE, "r");

 if (fp == NULL ){
   fprintf(stderr, "tcosxmlrpc::is_logged() file=\"%s\" no exists!!!\n", PASSWD_FILE);
   return xmlrpc_build_value(env, "s", NO_PASS_FILE);
 }
 else{
   /* read first line of PASSWD_FILE */

   /* fprintf(stderr, "tcosxmlrpc::is_logged() reading from file\n"); */
   fscanf(fp, "%s", login.data);
   fclose(fp);
   
   /* fprintf(stderr, "tcosxmlrpc::is_logged() login.data=\"%s\"\n", login.data); */
   
   split_login(plogin);
   /* fprintf(stderr, "tcosxmlrpc::is_logged() user=\"%s\" pass=\"%s\"\n", login.user, login.pass); */
   
 }
  /*------------------------------------------------------------*/

  if( strlen(user) != strlen(login.user) ){
     fprintf(stderr, "Diferent users %d\n", (strlen(user) - strlen(login.user)) );
     return xmlrpc_build_value(env, "s", NO_USER);
     }


  if (user == "" || pass == "")
    return xmlrpc_build_value(env, "s", NO_DATA);

  /* fprintf(stderr, "COMPARING user (%s)(%s)\n", user, login.user); */
  if( memcmp(user, login.user, sizeof(login.user) ) != 0 )
     return xmlrpc_build_value(env, "s", NO_USER);
     
  /* fprintf(stderr, "COMPARING pass \n(%s)\n(%s)\n", MDString(pass), login.pass); */
  if( memcmp(MDString(pass), login.pass, sizeof(login.pass) ) != 0 ){
     /* fprintf(stderr, "Password mismatch %d\n", strcmp(MDString(pass), login.pass) ); */
     return xmlrpc_build_value(env, "s", NO_PASS);
     }

  /* if here login OK */
  
  /* create a tmp file, is_logged delete every time is called (every method) */
  fp=fopen(LOGGED_FILE, "w");
  /* fprintf(fp, "%s:%s", user, pass); */
  fprintf(fp, "tcoxmlrpc logged\n");
  fclose(fp);
  return xmlrpc_build_value(env, "s", LOGIN_OK);
  
}



static xmlrpc_value *
tcos_logout(xmlrpc_env *env, xmlrpc_value *in, void *ud)
{
   return xmlrpc_build_value(env, "s", tcos_logout_helper() );
}




/*


void split_login( struct info *login);
int is_logged();
static xmlrpc_value * tcos_login(xmlrpc_env *env, xmlrpc_value *in, void *ud);
static xmlrpc_value * tcos_logout(xmlrpc_env *env, xmlrpc_value *in, void *ud);
char *tcos_logout_helper();

*/
