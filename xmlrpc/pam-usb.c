/*
* pam-usb.c part of tcosxmlrpc
*  => allow to login from GDM server using pam-usb
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

#include "pam-usb.h"
#include "xauth.h"

#if NEWAPI
xmlrpc_value *tcos_pam_usb(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_pam_usb(xmlrpc_env *env, xmlrpc_value *in, void *ud)
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
  /*char *fret;*/

  unsigned char *buf, *e;
  size_t len, elen;/*, fwret;*/
  static xmlrpc_value *result;

  dbgtcos("tcosxmlrpc::tcos_pam_usb() Init \n");

  /* read what option and cmdline params need */
  xmlrpc_parse_value(env, in, "(ssss)", &option, &cmdline, &cookie, &hostname);
  if (env->fault_occurred)
    return xmlrpc_build_value(env, "s", "params error");

  dbgtcos("tcosxmlrpc::tcos_pam_usb() option=%s cmdline=%s \n", option, cmdline);


  /* need XAUTH first (only if not initusb) */
  if ( strncmp(option, "initusb", 7) != 0 ) {
    dbgtcos("tcos_pam_usb() trying to read auth info\n");
    xauth_ok=handle_xauth(cookie,hostname);
    if( xauth_ok != XAUTH_OK ) {
        dbgtcos("tcos_pam_usb() error: xauth access denied :::::: err number: %d\n", xauth_ok);
        return xmlrpc_build_value(env, "s", "error: xauth access denied" );
    }
  }

  /* if option == getpad use base64 to read file */
  if ( strcmp(option, "getpad") == 0 ) {
    dbgtcos("tcos_pam_usb() option getpad, read base64 file\n");
    /* file is /mnt/__UUID__/.pamusb/__USER__.__SERVERHOSTNAME__.pad */
    buf = readfile(cmdline, &len);
    if (buf == NULL)
      return xmlrpc_build_value(env, "s", PAM_USB_EMPTY );
    e = base64_encode(buf, len, &elen);
    result = xmlrpc_build_value(env, "s", e );
    free(e);
    dbgtcos("tcos_pam_usb() option getpad, ok, returning base64 code (%d bytes)...\n", len);
    return result;
  }


  /* if option == savepad use base64 to write file */
  if ( strncmp(option, "/mnt/", 5) == 0 ) {
    dbgtcos("tcos_pam_usb() option savepad, save base64 file\n");
    /* file is /mnt/__UUID__/.pamusb/__USER__.__SERVERHOSTNAME__.pad */
    e = base64_encode( (const unsigned char *) cmdline, len, &elen);
    fp = fopen(option, "w");
    if (fp == NULL)
      return xmlrpc_build_value(env, "s", PAM_USB_READING_ERROR );
    (void)fwrite(e, 1, elen, fp);
    fclose(fp);
    free(e);
    dbgtcos("tcos_pam_usb() option savepad, ok, saving base64 code into '%s'...\n", option);
    return xmlrpc_build_value(env, "s", PAM_USB_OK );
  }

  /* if option == getpad use base64 to read file */
  if ( strncmp(option, "//mnt/", 6) == 0 ) {
    dbgtcos("tcos_pam_usb() option newpad, save base64 text into file in binary format\n");
    /* file is /mnt/__UUID__/.pamusb/__USER__.__SERVERHOSTNAME__.pad */
    len=(strlen(cmdline));
    e = base64_decode( (const unsigned char *) cmdline, len, &elen);
    fp = fopen(option, "w+");
    if (fp == NULL)
      return xmlrpc_build_value(env, "s", PAM_USB_READING_ERROR );
    (void)fwrite(e, 1, elen, fp);
    fclose(fp);
    free(e);
    dbgtcos("tcos_pam_usb() option newpad, ok, saving base64 code into '%s'...\n", option);
    return xmlrpc_build_value(env, "s", PAM_USB_OK );
  }


  snprintf( (char*) mycmd, BSIZE, "%s %s '%s'", PAM_USB_WRAPPER, option, cmdline);

  fp=(FILE*)popen( mycmd , "r");
  if (fp == NULL)
    return xmlrpc_build_value(env, "s", PAM_USB_READING_ERROR );

  /* put error msg into line var */
  strncpy(line, PAM_USB_ERROR, BSIZE);

  (void)fgets( line, sizeof line, fp);
  remove_line_break(line);
  pclose(fp);
  dbgtcos("tcosxmlrpc::tcos_pam_usb(%s) = %s \n", mycmd, line);

  if (env->fault_occurred) {
    return xmlrpc_build_value(env, "s", PAM_USB_READING_ERROR);
  }

  return xmlrpc_build_value(env, "s", line );
}




