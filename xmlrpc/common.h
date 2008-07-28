/*
* common.h part of tcosxmlrpc
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



#ifndef __COMMON_H__

#define IP_LENGHT 1024
#ifndef BSIZE
#define BSIZE 500
#endif
#define MY_IP_ADDRESS       TCOS_PATH"/getinfo.sh -i NETWORK_IP"

#define FILE_CONTENT_ERROR_MEM  "error: unable to malloc memory"
#define FILE_CONTENT_ERROR_FILE "error: file not found"

/* for split function */
#define MAXTOKENS 256


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

int mkstemp(char *template);
int snprintf(char *str, size_t size, const char *format, ...);


#endif
