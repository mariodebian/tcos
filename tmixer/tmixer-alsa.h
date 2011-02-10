/*
 *   ALSA command line mixer utility
 *   Copyright (c) 1999-2000 by Jaroslav Kysela <perex@perex.cz>
 *
 *   Modifications to show parseable output by:
 *   Mario Izquierdo (mariodebian) <mariodebian@gmail.com>
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



/** COPY FROM ../include/version.h *****************************/
#define SND_UTIL_VERSION_STR    "1.0.16"



#define LEVEL_BASIC		(1<<0)
#define LEVEL_INACTIVE		(1<<1)
#define LEVEL_ID		(1<<2)

#include <alloca.h>
void *alloca( size_t  size );

#include <math.h>
double rint(double x);


#include <strings.h>
int strncasecmp(const char *s1, const char *s2, size_t n);
