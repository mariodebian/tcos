/*
* screenshot.c part of tcosxmlrpc
*   => method exec scrot to make an screenshot
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

#include "screenshot.h"


#if NEWAPI
static xmlrpc_value *tcos_get_screenshot(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
static xmlrpc_value *tcos_get_screenshot(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
    {
    
    FILE *fp;
    unsigned int scrot_size;
    
    char line[BIG_BUFFER];
    char *bufferb64;
    char *size;
    char *user;
    char *pass;
    char *login_ok;
    
    static xmlrpc_value *result;

    /* read what info search */
    xmlrpc_parse_value(env, in, "(sss)", &size, &user, &pass);
    if (env->fault_occurred)
        return xmlrpc_build_value(env, "(ss)", "error: params error", SCROT_EMPTY);

    /* need login first */
    login_ok=validate_login(user,pass);
    if( strcmp(login_ok,  LOGIN_OK ) != 0 )
        return xmlrpc_build_value(env, "(ss)", login_ok, SCROT_EMPTY );

    if ( strlen(size) > 0)
        snprintf( (char*) line, BSIZE, "%s %s", SCROT_CMD_B64, size);
    else
        snprintf( (char*) line, BSIZE, "%s", SCROT_CMD_B64);

    dbgtcos("tcosxmlrpc::getscreenshot() exe=%s\n", line);

    /* exec command (to generate b64 text file) */
    fp=(FILE*)popen(line, "r");
    fclose(fp);
     
    /* get file size and reserve memory */
    scrot_size=getfilesize(SCREENSHOT_FILE);
    dbgtcos("tcosxmlrpc::getscreenshot() file size %d\n", scrot_size);
    
    /* reserve memory */
    dbgtcos("tcosxmlrpc::getscreenshot() trying to reserve %d\n", sizeof(char)*(scrot_size +1));
    bufferb64 = (char*) malloc (sizeof(char)*scrot_size +1);
    if ( bufferb64 == NULL ) {
        dbgtcos("tcosxmlrpc::getscreenshot() memory error\n");
        return xmlrpc_build_value(env, "(ss)", SCROT_ERROR_MEM, SCROT_EMPTY);
    }
    
    /* read line (base64 have 1 line only) */
    fp = fopen(SCREENSHOT_FILE, "r");
    if ( fp == NULL ) {
        dbgtcos("tcosxmlrpc::getscreenshot() file error\n");
        return xmlrpc_build_value(env, "(ss)", SCROT_ERROR_FILE, SCROT_EMPTY);
    }
    fgets(bufferb64, sizeof(char)*(scrot_size +1), fp);
    fclose(fp);
     
    /* build result */
    result = xmlrpc_build_value(env, "(ss)", SCROT_OK, bufferb64 );
    
    /* free buffer */
    free(bufferb64);

    return result;
}

