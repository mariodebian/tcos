/*# tcosmd5.c helper to MD% an string 2006-09-09 14:22:40 mariodebian $
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
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "md5/global.h"
#include "md5/md5.h"

#ifndef MD
#define MD 5
#endif


#define MD5_CTX MD5_CTX
#define MDInit MD5Init
#define MDUpdate MD5Update
#define MDFinal MD5Final

/* Length of test block, number of test blocks.
 */
#define TEST_BLOCK_LEN 1000
#define TEST_BLOCK_COUNT 1000

static char *MDString PROTO_LIST ((char *));

char _md5[1000];

/* Digests a string and prints the result.
 */
static char *MDString (string)
char *string;
{
  unsigned int i;
  char _tmp[1000];

  MD5_CTX context;
  unsigned char digest[16];
  unsigned int len = strlen (string);

  MDInit (&context);
  MDUpdate (&context, string, len);
  MDFinal (digest, &context);

  for (i = 0; i < 16; i++)
  {
  snprintf( (char*) &_tmp, 1000, "%s%02x", _md5, digest[i]);
  snprintf( (char*) &_md5, 1000, "%s", _tmp);
  /* printf ("%02x", digest[i]); */
  }
  return(_md5);
}

int main(int argc, char **argv){

char *_md5;

if (argc != 2) {
	fprintf(stderr, "Usage:\n\t tcosmd5 string\n");
	return(1);
    }
 _md5=MDString(argv[1]);
 printf("%s\n", _md5);
 
 return(0);
}
