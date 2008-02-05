/*# common.c  2006-09-09 14:22:40 mariodebian $
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

#include "common.h"


struct ip_address check_ip_address(char *data) {
  struct ip_address ip;
  /*
  *  split ip address with 4 dots
  *  join ip address in ip.ipstr
  *  and compare with original data
  *
  */
  sscanf(data, "%d.%d.%d.%d", &ip.data[0], &ip.data[1], &ip.data[2], &ip.data[3]);

  snprintf(ip.ipstr, IP_LENGHT, "%d.%d.%d.%d", ip.data[0], ip.data[1], ip.data[2], ip.data[3]);

  dbgtcos("tcosxmlrpc::is_ip() comparing %s with %s ...\n", data, ip.ipstr);

  if ( strcmp(data, ip.ipstr) == 0 ) { 
    ip.is_ip=1;
  }
  else {
    ip.is_ip=0;
  }
  return ip;
}


int
check_for_file(char *fname)
{
/* return 0 if not found or cant read or 1 if found */

 FILE *fptemp;
 fptemp = fopen(fname,"r");
 if (!fptemp) {
    dbgtcos("info check_for_file(): File \"%s\" not found or cant read.\n", fname);
    return 0;
 }
 else
    dbgtcos("info check_for_file(): File \"%s\" found.\n", fname);
   return 1;
}


