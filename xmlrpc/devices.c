/*
* devices.c part of tcosxmlrpc
*  => return all block devices and set/get actions
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
#include "devices.h"
#include "xauth.h"

#if NEWAPI
xmlrpc_value *tcos_devices(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_devices(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
 {
  FILE *fp;
  char line[BSIZE];
  char *option;
  char *cmdline;
  char mycmd[BSIZE];
  char *cookie;
  char *hostname;
  int xauth_ok;
  char *fret;

  dbgtcos("tcosxmlrpc::tcos_devices() Init \n");

  /* read what option and cmdline params need */
  xmlrpc_parse_value(env, in, "(ssss)", &option, &cmdline, &cookie, &hostname);
  if (env->fault_occurred)
        return xmlrpc_build_value(env, "s", "params error");

   dbgtcos("tcosxmlrpc::tcos_devices() option=%s cmdline=%s \n", option, cmdline);

  if (strcmp(option, "--getxdrivers") == 0 ) {
     dbgtcos("tcosxmlrpc::tcos_devices() --getxdrivers no xauth\n");
  }
  else {  
    /* need XAUTH first */
    xauth_ok=handle_xauth(cookie,hostname);
    if( xauth_ok != XAUTH_OK ) {
          dbgtcos("tcos_devices() error: xauth access denied :::::: err number: %d\n", xauth_ok);
          return xmlrpc_build_value(env, "s", "error: xauth access denied" );
      }
  }

   /*dbgtcos("tcosxmlrpc::tcos_devices() exec=%s %s %s \n", DEVICES_WRAPPER, option, cmdline);*/

   snprintf( (char*) mycmd, BSIZE, "%s %s %s", DEVICES_WRAPPER, option, cmdline);

   fp=(FILE*)popen( mycmd , "r");
   if (fp == NULL)
      return xmlrpc_build_value(env, "s", DEVICES_READING_ERROR );

   /* put error msg into line var */
   strncpy(line, DEVICES_ERROR, BSIZE);

   fret = fgets( line, sizeof line, fp);
   remove_line_break(line);
   pclose(fp);
   dbgtcos("tcosxmlrpc::tcos_devices(%s) = %s \n", mycmd, line);

   if (env->fault_occurred) {
        return xmlrpc_build_value(env, "s", DEVICES_READING_ERROR);
   }

   
   return xmlrpc_build_value(env, "s", line );

}




