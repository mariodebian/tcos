/*# common.h  2006-09-09 14:22:40 mariodebian $
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



#ifndef __COMMON_H__

#define IP_LENGHT 1024
#ifndef BSIZE
#define BSIZE 500
#endif
#define MY_IP_ADDRESS       TCOS_PATH"/getinfo.sh -i NETWORK_IP"


typedef struct ip_address {
  int data[4] ;
  int is_ip;
  char ipstr[IP_LENGHT];
} ip ;

#define  __COMMON_H__



FILE *popen(const char *orden, const char *tipo);
int pclose(FILE *flujo);

int gethostname(char *name, size_t len);
int setenv(const char *nombre, const char *valor, int sobrescribir);
void unsetenv(const char *nombre);

char *mkstemp(char *template);
int snprintf(char *str, size_t size, const char *format, ...);


#endif
