/*
* debug.c part of tcosxmlrpc
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


/*
*  you can compile this file as standalone app with:
*     gcc -Wall debug.c -o debug
*
*  Try exec:
*     ./debug   [nothing to stdout or err]
*     TCOS_DEBUG=1 ./debug  print some debug strings
*/

#include <stdio.h>

/* variable argument list */
#include <stdarg.h>

/* getenv */
#include <stdlib.h>

/* strcmp */
#include <string.h>

#ifndef HAVE_DEBUG
#define dbgtcos(s, ...) __dbgtcos(__FILE__, __LINE__, s, ##__VA_ARGS__)
#define HAVE_DEBUG 1
#endif

void __dbgtcos(const char *file, int line, const char *format_str, ... );
