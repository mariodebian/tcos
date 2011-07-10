/*
* mount_listener.c poll file and get changes using a tmp file
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

/*

  Usage:
     mount-listener /proc/mounts /tmp/proc_mounts

   listen in changes of /proc/mounts and exec SAVE_UDEV script

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <poll.h>
#include <fcntl.h>

/* variable argument list */
#include <stdarg.h>

#define MSG_BUFF 4096
#define POLL_TIMEOUT 2*1000



int snprintf(char *str, size_t size, const char *format, ...);
FILE *popen(const char *cmd, const char *type);
int pclose(FILE *fp);

#include <string.h>
char *strtok(char *str, const char *delim);


#include <sys/stat.h>
int file_exists (char * fileName)
{
    struct stat buf;
    int i = stat ( fileName, &buf );
    /* File found */
    if ( i == 0 )
        return 1;
    
    return 0;
}

#include <time.h>
void mysleep(time_t sec, long nsec){
    struct timespec ourtime;
    ourtime.tv_sec=sec;
    ourtime.tv_nsec=nsec;
    nanosleep(&ourtime, NULL);
}

#define SAVE_UDEV "/usr/sbin/save-udev.sh"
int found_device=0;


void debug( const char *format_str, ... ) {
    va_list ap;
    va_start( ap, format_str );
    va_end( ap );
    vfprintf(stderr, format_str , ap);
}


int getnumlines( char *fname )
{
    FILE *fp;
    int lines=0;
    char line[MSG_BUFF];
    fp=fopen(fname, "r");
    if(!fp) return -1;
    while ( fgets(line, MSG_BUFF, fp) != NULL ) {
        ++lines;
    }
    fclose(fp);
    /*debug("  DEBUG: getnumlines of '%s' %d\n", fname, lines);*/
    return lines;
}



void print_dev(char *txt, char *action) {
    char *output;
    char cmd[MSG_BUFF];
    FILE *fp;
    
    /* use strtok to split txt with spaces */
    output=strtok(txt, " ");
    
    debug("DEBUG: output \"%s %s\"\n", output, action);

    if ( strncmp(output , "/dev/disk/", 10) == 0 ) {
        debug("DEBUG: detected /dev/disk device => pam-usb IGNORING...\n");
        return;
    }

    snprintf( (char*) &cmd, MSG_BUFF, "%s %s %s", SAVE_UDEV, output, action);

    if ( file_exists(SAVE_UDEV) ) {
        debug("    DEBUG: command \"%s\"\n", cmd);
        fp=popen(cmd, "r");
        pclose(fp);
    }
    else {
        debug("ERROR !!! can't find %s script to exec %s\n", SAVE_UDEV, cmd);
    }
    debug("    DEBUG: End of print_dev()\n");
}


void sync_files(char *fname1, char *fname2){
    FILE *in, *out;
    char ch;

    if((in=fopen(fname1, "rb")) == NULL) {
        debug("Cannot open input file.\n");
        return;
    }
    if((out=fopen(fname2, "wb")) == NULL) {
        debug("Cannot open output file.\n");
        return;
    }

    while(!feof(in)) {
        ch = getc(in);
        if(ferror(in)) {
          debug("Read Error");
          clearerr(in);
          break;
        } else {
          if(!feof(in)) putc(ch, out);
          if(ferror(out)) {
            debug("Write Error");
            clearerr(out);
            break;
          }
        }
    }
    fclose(in);
    fclose(out);
    debug(" DEBUG: sync_files( %s => %s ) done...\n", fname1, fname2);
}



int compare(char *fname1, char *fname2)
{
    int n1, n2;
    char lineold[MSG_BUFF];
    char linenew[MSG_BUFF];
    char *old, *new;
    FILE *fpold, *fpnew;
    int dev=0;
    char *action="";

    n1=n2=0;
    n1=getnumlines(fname1);
    n2=getnumlines(fname2);
    
    if (n1 == -1 || n2 == -1) {
        debug("DEBUG: error reading number of lines n1=%d, n2=%d\n", n1, n2);
        return -1;
    }

    if (n1 == n2) {
        /*debug("DEBUG: warning, files have the same number of lines %d\n", n1);*/
        return 1;
    }

    if (n1 > n2) {
        /* mount */
        debug("   DEBUG: compare MOUNT, diff=%d\n", n1-n2);
        old=fname1;
        new=fname2;
        action="mount";
    }
    else {
        /* umount */
        debug("   DEBUG: compare UMOUNT, diff=%d\n", n2-n1);
        old=fname2;
        new=fname1;
        action="umount";
    }

    /* open file that contain extra lines */
    fpold=fopen(old, "r");
    while ( fgets(lineold, MSG_BUFF, fpold) != NULL ) {
        dev=0;
        fpnew=fopen(new, "r");
        while (fgets(linenew, MSG_BUFF, fpnew) != NULL ) {
            if ( strcmp(linenew, lineold) == 0) {
                /*debug("   DEBUG: linenew \"%s\"found\n", linenew);*/
                dev=1;
                break;
            }
        }

        if (dev == 0) {
            debug("   DEBUG: lineold not found %s", lineold);
            print_dev(lineold, action);
        }
        fclose(fpnew);
    }
    fclose(fpold);
    debug("  DEBUG: end of compare() \n");
    sync_files(fname1, fname2);
    return 0;
}


int main (int argc, char *argv[]) {
    int fd_file;
    struct pollfd fdarray;
    int rc;
    debug("DEBUG: *** comparing: %s <=> %s\n", argv[1], argv[2]);

    sync_files(argv[1], argv[2]);

    if ((fd_file = open(argv[1], O_RDONLY, 0)) < 0) {
        perror("Error opening file");
        return -1;
    }

    for (;;) {
        fdarray.fd = fd_file;
        fdarray.events = POLLIN | POLLERR;

        rc = poll(&fdarray, 1, POLL_TIMEOUT);

        if (rc < 0) {
            perror("error reading poll() \n");
            return -1;
        }

        else if(rc > 0) {
            /*debug("  DEBUG: Changes detected at %s rc=%d revents=%d\n", argv[1], rc, fdarray.revents);*/
            mysleep(0, 2000);
            /* ugly hack to not eat all CPU when poll() return inmediatly */
            if ( compare(argv[1], argv[2]) == 1){
                /*printf("      sleeping 2 seconds \n");*/
                mysleep(2, 0);
            }

        }
	/* poll() return 0 if no changes
        else {
            perror("ret value of poll() unknow\n");
        }*/
    }

    /* never here */
    return 0;
}



