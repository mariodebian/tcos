/*
* waitforX.c
* Copyright (C) 2011  mariodebian at gmail
*
*  wait for X server to start up
*
*   iTALC/Client-software
*   Copyright (c) 2004-2005 Tobias Doerffel <tobias@doerffel.de>
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

#include <unistd.h>
#include <stdio.h>
#include <X11/Xlib.h>

/* number of tries to open X display */
#define MAX_TRY 25

/* wait 1/5 second */
#define WAIT_LOOP 200000

void usleep(unsigned long usec);

int main(int argc, char **argv) {

    int i=0;
    Display *xd=NULL;
    /* allow to pass as argument the DISPLAY var */
    char *displayNum=":0";
    if (argc == 2) {
        displayNum=argv[1];
    }
    #ifdef DEBUG
    fprintf(stderr, "Opening DISPLAY '%s'\n",displayNum);
    #endif

    for ( i=0; i< MAX_TRY; i++ ) {
        if ( (xd=XOpenDisplay(displayNum) ) ) {
            #ifdef DEBUG
            fprintf(stderr, "DISPLAY '%s' opened in %d iterations done, return 0\n",displayNum, i);
            #endif
            XCloseDisplay(xd);
            return 0;
        }
        #ifdef DEBUG
        fprintf(stderr, "DISPLAY '%s' not opened in %d iterations\n",displayNum, i);
        #endif
        usleep(WAIT_LOOP);
    }

    #ifdef DEBUG
    fprintf(stderr, "DISPLAY '%s' not opened, return 1\n",displayNum);
    #endif
    return 1;
}
