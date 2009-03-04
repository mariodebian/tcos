/*
* validate.h part of tcosxmlrpc
*   => common headers of validate.c
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




#ifndef __VALIDATE_H__

/* crypt salt */
#define PASS_ID "$1$id2trA1q"

#define LOGIN_OK      "ok"
#define LOGIN_NOUSER  "error: User not allowed"
#define LOGIN_NOPASS  "error: Password not valid"
#define LOGIN_NOPERMS "error: cant read shadow"
#define LOGIN_EXPIRED "error: user expired"
#define LOGIN_ERROR   "error: unknow error"
#define LOGIN_SHADOW_ERROR "error: can't read /etc/shadow"


/* USE_XOPEN makes crypt available from unistd.h. */
#define __USE_XOPEN
#include <unistd.h>
char *crypt(const char *key, const char *salt);

typedef struct info {
  char line[BSIZE];
  char *user;
  char *pass;
  int logged;
} login ;


#define __VALIDATE_H__

/* The longest allowable length of a username */
#define MAX_USERNAME_LENGTH 100

/* The longest allowable length of the plaintext password*/
#define MAX_PW_LENGTH 100

/* How many seconds to sleep on a failed validation */
#define SLEEP_SECONDS (0)

/* Whether or not to record failed attempts in the system log defined=yes, not defined=no */
#define LOG_FAILED_ATTEMPTS 

#endif

