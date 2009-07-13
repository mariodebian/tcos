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

#include "common.h"
#include "debug.h"
#include "validate.h"
#include "screenshot.h"


#if NEWAPI
xmlrpc_value *tcos_get_screenshot(xmlrpc_env *const env, xmlrpc_value *const in, void *const serverContext)
#else
xmlrpc_value *tcos_get_screenshot(xmlrpc_env *env, xmlrpc_value *in, void *ud)
#endif
    {
    
    FILE *fp;
    
    char line[BIG_BUFFER];
    char *size;
    char *user;
    char *pass;
    char *login_ok;

    size_t len, elen;
	unsigned char *buf, *e;
    
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
        snprintf( (char*) line, BSIZE, "%s %s", SCROT_CMD, size);
    else
        snprintf( (char*) line, BSIZE, "%s", SCROT_CMD);

    dbgtcos("tcosxmlrpc::getscreenshot() exe=%s\n", line);

    /* exe screenshot */
    fp=(FILE*)popen(line, "r");
    fclose(fp);

    /* convert base64 string */
    dbgtcos("tcosxmlrpc::getscreenshot() reading file=%s\n", SCREENSHOT_FILE);
    buf = readfile(SCREENSHOT_FILE, &len);
	if (buf == NULL)
		return xmlrpc_build_value(env, "(ss)", SCROT_ERROR, SCROT_EMPTY );

	e = base64_encode(buf, len, &elen);

    result = xmlrpc_build_value(env, "(ss)", SCROT_OK, e );
    
    free(e);
    return result;
}

