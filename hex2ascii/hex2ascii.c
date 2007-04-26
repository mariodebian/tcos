/*# hex2ascii.c convert a hex string into ascii string 2006-09-09 14:22:40 mariodebian $
#
# This file is part of initramfs-tools-tcos.
#
# initramfs-tools-tcos is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# initramfs-tools-tcos is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with initramfs-tools-tcos; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.
*/

/*
* Mario Izquierdo mariodebian@gmail.com
* Based on http://www.bigbold.com/snippets/posts/show/2073
*
*/

#include <stdio.h>
#include <stdlib.h>


#include <string.h>
char *strtok(char *s, const char *delim);



char hexToAscii(char *io_hex)
{
	/*printf("DEBUG: %s\n", io_hex);*/
	char hex[5], *stop;
	hex[0] = '0';
	hex[1] = 'x';
	hex[2] = io_hex[0];
	hex[3] = io_hex[1];
	hex[4] = 0;
	return strtol(hex, &stop, 16);
}

void usage()
{
  printf("hex2ascii converter usage:\n\n");
  printf("          hex2ascii xx:xx:xx:xx:xx:xx\n");
}

int main(int argc, char* argv[])
{
  int i;
  char *p;

  if ( argc==1 ) {
     usage();
     return(1);
  }

  /* split by : */
  p = (char*) strtok ( argv[1],":");
  i=0;
  while (p != NULL)
  {
   printf("%c", hexToAscii(p) );
   p = (char*) strtok (NULL, ":");
  }
  printf("\n");
  return(0);
}

