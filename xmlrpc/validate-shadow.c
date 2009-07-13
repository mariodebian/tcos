/*
* validate-shadow.c part of tcosxmlrpc
*   => Validate a username/pw combination, using the /etc/shadow file
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


#include "common.h"
#include "debug.h"
#include "validate.h"

#include <shadow.h>
#include <string.h>


char
*validate_shadow(char *user, char *pw)
{
  FILE* fptemp;
  struct spwd *ret = NULL;
  char *cryptpw;
  long int secs, days;


  #ifdef VISIBLE_PASSWD
    dbgtcos( "info validate_shadow(): user=%s pass=%s\n", user, pw);
  #else
    dbgtcos( "info validate_shadow(): user=%s pass=**NOT-SHOW**\n", user);
  #endif


  ret = getspnam(user);

  if (!ret) {
    /* Did the user not exist?  Or can we not read /etc/shadow?
     * Print a helpful message if the latter...
     */
    fptemp = fopen("/etc/shadow","r");
    if (!fptemp) {
      fprintf(stderr, "error validate_sadow(): No read access to /etc/shadow. no running as root?.\n");
      return( (char*) LOGIN_NOPERMS);
    }

    fclose(fptemp);

    fprintf(stderr,"error validate_shadow(): Couldn't find user '%s'\n",user);

    if ( (SLEEP_SECONDS>0) && (sleep(SLEEP_SECONDS)!=0))
      fprintf(stderr,"error validate_shadow(): Error sleeping for %d seconds.\n", SLEEP_SECONDS);

    return( (char*) LOGIN_NOUSER);
  }

  cryptpw = (char*) crypt(pw,ret->sp_pwdp);
  if (strcmp(cryptpw,ret->sp_pwdp)!=0){

    if ( (SLEEP_SECONDS>0) && (sleep(SLEEP_SECONDS)!=0))
      fprintf(stderr,"error validate_shadow(): Error sleeping for %d seconds.\n", SLEEP_SECONDS);

    fprintf(stderr,"error validate_shadow(): User %s: authentication failure\n",user);
    return((char*) LOGIN_NOPASS);
  }

  /*
   * Check password expiration information. See shadow(5)
   */
  secs = (long int) time(NULL);
  days = secs / (24L * 60L * 60L);
  /*
   *   An  account is considered to be inactive and is disabled if the
   *   password is not changed within the specified number of days after the
   *   password expires.
   */
  if ( ret->sp_max > 0
       && ret->sp_inact > 0
       && days > ret->sp_lstchg + ret->sp_max + ret->sp_inact) {

    if ( (SLEEP_SECONDS>0) && (sleep(SLEEP_SECONDS)!=0))
      fprintf(stderr,"error validate_shadow(): Error sleeping for %d seconds.\n", SLEEP_SECONDS);

    fprintf(stderr,"error validate_shadow(): User %s: account inactivity period expired\n",user);
    return((char*) LOGIN_EXPIRED);
  }

  /*
   * An account will also be disabled  on the specified day regardless of
   * other password expiration information.
   */
  if (ret->sp_expire > 0 && days > ret->sp_expire) {

    if ( (SLEEP_SECONDS>0) && (sleep(SLEEP_SECONDS)!=0))
      fprintf(stderr,"error validate_shadow(): Error sleeping for %d seconds.\n", SLEEP_SECONDS);

    fprintf(stderr,"error validate_shadow(): User %s: account expired\n",user);
    return((char*) LOGIN_EXPIRED);
  }

  dbgtcos("info validate_shadow(): login correct\n");

  return((char*) LOGIN_OK);
}


