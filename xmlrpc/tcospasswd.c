/*
* tcospasswd.c part of tcosxmlrpc
*   => return an crypt passwd
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

#include <stdio.h>
#include <string.h>


#include <pwd.h>
#include "validate.h"

void usage();

int main( int argc, char **argv) {

 char *pass;
 if (argc==2) {
   printf("%s\n", crypt(argv[1], PASS_ID) );
   return 0;
 }

 if (argc != 2) {
   pass=getpass("TCOS Password: ");
   printf("%s\n", crypt(pass, PASS_ID) );
   return 1;
 }

 usage();
 return 1;
}


void
usage()
{
  printf("Usage:\n\t\t tcospasswd pass\n");
}


