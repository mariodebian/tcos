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


#ifndef HAVE_DEBUG
#define dbgtcos(s, ...) __dbgtcos(__FILE__, __LINE__, s, ##__VA_ARGS__)

void __dbgtcos(const char *file, int line, const char *format_str, ... ) {
  if ( getenv("TCOS_DEBUG") == NULL) return;
  if ( strcmp(getenv("TCOS_DEBUG"), "1" ) == 0  ) {
    va_list ap;
    fprintf(stderr, "[%s:%03d] ", file, line);
    va_start( ap, format_str );
    va_end( ap );
    vfprintf(stderr, format_str , ap);
  }
}
#define HAVE_DEBUG 1
#endif
/*
#ifndef HAVE_MAIN
int main( int argc, char **argv ) {
 
  dbgtcos ( "testing debug is ON\n" );
  dbgtcos ( "Mi username is %s and my home path is %s\n", getenv("USERNAME"),  getenv("HOME") );
  dbgtcos ( "My XAUTHORITY is %s\n", getenv("XAUTHORITY") );
  dbgtcos ( "Var not found test %s\n", getenv("NOTFOUND") );
  return(0);
}
#endif
*/
