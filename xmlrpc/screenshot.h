/*
* screenshot.h part of tcosxmlrpc
*   => common headers of screenshot.c
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


/* xmlrpc methods to screenshot thin client display */

#define SCROT_CMD     TCOS_PATH"/screenshot.sh"
#define SCROT_CMD_B64 TCOS_PATH"/screenshot64.sh"

#ifdef IS_STANDALONE
#define SCREENSHOT_FILE TCOS_PATH"/var/www/capture-thumb.b64"
#else
#define SCREENSHOT_FILE "/var/www/capture-thumb.b64"
#endif

#define SCROT_ERROR "error: screenshot command error"
#define SCROT_ERROR_FILE "error: screenshot file not found"
#define SCROT_ERROR_MEM  "error: screenshot memory error"
#define SCROT_ERROR_BUF  "error: screenshot buffer error"

#define SCROT_OK "ok"
#define SCROT_EMPTY ""


