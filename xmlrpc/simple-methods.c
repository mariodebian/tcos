/*
* simple-methods.c part of tcosxmlrpc
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


/* tcosxmlrpc simple methods */

#include "common.h"
#include "debug.h"
#include "validate.h"

#if NEWAPI
xmlrpc_value *tcos_version(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_version(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
    dbgtcos("tcosxmlrpc::tcos_version() %s\n", VERSION);
    return xmlrpc_build_value(env, "s", VERSION);
 }


#if NEWAPI
xmlrpc_value *tcos_echo(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_echo(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
   char *s;
   size_t *len;
   xmlrpc_parse_value(env, in, "(s#)", &s, &len);
   dbgtcos("tcosxmlrpc::tcos_echo() %s\n", s);
   return xmlrpc_build_value(env, "s", s);
 }


#if NEWAPI
xmlrpc_value *tcos_status(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_status (xmlrpc_env *env, xmlrpc_value *in, void *user_data)
#endif
{
  FILE *fp=NULL;
  char *app=NULL;
  char cmd[BUFF_SIZE];
  char ret[BUFF_SIZE];
  /*int fret;*/


  dbgtcos("tcosxmlrpc::tcos_status() Init \n");

  /* Parse app string */
  xmlrpc_parse_value(env, in, "(s)", &app);

  if (env->fault_occurred)
    return xmlrpc_build_value(env, "s", "params error");

  dbgtcos("tcosxmlrpc::tcos_status() pidof %s\n", app);
  snprintf( (char*) &cmd, BUFF_SIZE, "pidof %s| grep -c \"[1234567890]\"",app);
  dbgtcos("tcosxmlrpc::tcos_status() exec cmd=\"%s\"\n", cmd);

  fp=(FILE*)popen(cmd, "r");
  if (env->fault_occurred) {
    pclose(fp);
    return xmlrpc_build_value(env, "s", "exec error");
  }

  dbgtcos("tcosxmlrpc::tcos_status() reading from fp pointer\n");
  (void)fscanf(fp, "%s", ret);
  dbgtcos( "tcosxmlrpc::tcos_status() ret value=%s\n", ret);
  pclose(fp);

  if (ret != NULL)
    return xmlrpc_build_value(env, "s", ret);
  else
    return xmlrpc_build_value(env, "s", "error");
}
