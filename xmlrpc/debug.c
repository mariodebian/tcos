/*# debug.c  2006-09-09 14:22:40 mariodebian $
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

/*
*  you can compile this file as standalone app with:
*     gcc -Wall debug.c -o debug
*
*  Try exec:
*     ./debug   [nothing to stdout or err]
*     TCOS_DEBUG=1 ./debug  print some debug strings
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* variable argument list */
#include <stdarg.h>

#ifndef HAVE_DEBUG
void dbgtcos( const char *format_str, ... ) {
  if ( getenv("TCOS_DEBUG") == NULL) return;
  if ( strcmp(getenv("TCOS_DEBUG"), "1" ) == 0  ) {
    va_list ap;
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
