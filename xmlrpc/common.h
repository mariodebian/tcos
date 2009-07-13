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
#define  __COMMON_H__

#define IP_LENGHT 1024
#ifndef BSIZE
#define BSIZE 500
#endif
#define BUFF_SIZE 1000
#define BIG_BUFFER 5000

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




/* common.c */
void remove_line_break( char *s );

#include <sys/types.h>
unsigned char *readfile( const char *name, size_t *len );
unsigned char * base64_encode( const unsigned char *src, size_t len, size_t *out_len );
unsigned char * base64_decode(const unsigned char *src, size_t len, size_t *out_len);
struct ip_address check_ip_address(char *data);
char **split(char *string, char *delim);
int check_for_file(char *fname);


#include <xmlrpc.h>
#include <xmlrpc_abyss.h>

#define OLD_VERSION_MAYOR 0
#define OLD_VERSION_MINOR 920

#if XMLRPC_VERSION_MAYOR > OLD_VERSION_MAYOR || ( (XMLRPC_VERSION_MAYOR == OLD_VERSION_MAYOR) && (XMLRPC_VERSION_MINOR > OLD_VERSION_MINOR) )
  #define NEWAPI 1
  #include <xmlrpc_server.h>
#endif



#endif
