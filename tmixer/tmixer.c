/*  tmixer main app
 *
 *   Detect OSS and ALSA and pass argc and argv to tmixer_oss or tmixer_alsa
 *   copyright (c) Mario Izquierdo (mariodebian) <mariodebian@gmail.com>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define HAVE_TMIXER_MAIN 1

#include "tmixer-oss.c"
#include "tmixer-alsa.c"

#include <sys/stat.h>
int file_exists (char * fileName) {
    struct stat buf;
    int i = stat ( fileName, &buf );
    /* File found */
    if ( i == 0 )
        return 1;
    return 0;
}


int main(int argc, char *argv[]) {
    if ( getenv("TMIXER_FORCE") != NULL ) {
        if( strcmp(getenv("TMIXER_FORCE"), "oss") == 0 ) {
            /* force to work with OSS/aumix api */
            return tmixer_oss(argc, argv);
        }
        if( strcmp(getenv("TMIXER_FORCE"), "alsa") == 0 ) {
            /* force to work with ALSA/amixer api */
            return tmixer_alsa(argc, argv);
        }
    }
    /* AUTOMATIC DETECT
     *    1.- Search for ALSA ( exists /proc/asound/card0/id )
     *    2.- Search for OSS  ( exists /dev/dsp and /dev/mixer )
     */
    if ( file_exists ("/proc/asound/card0/id") ) {
        return tmixer_alsa(argc, argv);
    }
    else if ( file_exists ("/dev/dsp") && file_exists ("/dev/mixer") ) {
        return tmixer_oss(argc, argv);
    }
    else {
        fprintf(stderr, "tmixer: ERROR, can't detect ALSA or OSS devices.\n");
        return 1;
    }
}
