/*
* exe.c part of tcosxmlrpc
*   => method that exec an app
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


#include "common.h"
#include "debug.h"
#include "validate.h"
#include "exe.h"


void
job_exe( char *cmd )
{
  FILE *fp;
  char job[BUFF_SIZE];

  dbgtcos("tcosxmlrpc::job_exe() exec=> \"%s\"\n", cmd);

  snprintf( (char*) &job, BUFF_SIZE, "%s %s", CMD_WRAPPER, cmd );

  fp=(FILE*)popen(job, "r");
  pclose(fp);

  dbgtcos("tcosxmlrpc::job_exe() EXEC cmd=\"%s\"\n", job);
  return;
}

void
kill_exe( char *cmd )
{
  FILE *fp;
  char job[BUFF_SIZE];

  snprintf( (char*) &job, BUFF_SIZE, "killall %s", cmd );

  dbgtcos("tcosxmlrpc::kill_exe() exec=> \"%s\"\n", job);

  fp=(FILE*)popen(job, "r");
  pclose(fp);
}


#if NEWAPI
xmlrpc_value *tcos_exe(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_exe(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
{
     char *s;
     char *user;
     char *pass;
     char *login_ok;

  dbgtcos("tcosxmlrpc::tcos_exe() Init \n");

  xmlrpc_parse_value(env, in, "(sss)", &s, &user, &pass);
  if (env->fault_occurred)
    return xmlrpc_build_value(env, "s", "error: params error");


  /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  dbgtcos("tcosxmlrpc::tcos_exe s=\"%s\" \n", s);

  job_exe(s);

  return xmlrpc_build_value(env, "s", s);
}

#if NEWAPI
xmlrpc_value *tcos_kill(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_kill(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
{
  char *s;
  char *user;
  char *pass;
  char *login_ok;

  dbgtcos("tcosxmlrpc::tcos_kill() Init \n");

  xmlrpc_parse_value(env, in, "(sss)", &s, &user, &pass);
  if (env->fault_occurred)
    return xmlrpc_build_value(env, "s", "error: params error");;

  /* need login first */
  login_ok=validate_login(user,pass);
  if( strcmp(login_ok,  LOGIN_OK ) != 0 )
    return xmlrpc_build_value(env, "s", login_ok );

  dbgtcos("tcosxmlrpc::tcos_kill s=\"%s\" \n", s);

  kill_exe(s);

  return xmlrpc_build_value(env, "s", s);
}


