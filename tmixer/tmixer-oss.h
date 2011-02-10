/* $Aumix: aumix/src/common.c,v 1.9 2002/10/29 21:27:51 trevor Exp $
 *
 * aumix:  adjust audio mixer
 * copyright (c) 1993, 1996-2002 the authors--see AUTHORS file
 *
 * This file is part of aumix.
 *
 * Modified to merge with amixer by Mario Izquierdo (mariodebian) <mariodebian@gmail.com>
 *
 * Aumix is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * Aumix is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */



#define OSSMAXLEVEL 100		/* highest level permitted by OSS drivers */

#define OSSUNMUTELEVEL 85	/* level of unmute (no way to get old level) */
/* you can overwrite this level with TMIXER_OSSMUTELEVEL environment var */


#define SOUND_IOCTL(a,b,c)	ioctl(a,b,c)

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

enum {
	ENOERROR,
	EOPENMIX,		/* trouble opening mixer device */
	EFINDDEVICE,	/* no device found */
	EREADDEV,		/* SOUND_MIXER_READ_DEVMASK */
	EREADRECMASK,	/* SOUND_MIXER_READ_RECMASK */
	EREADRECSRC,	/* SOUND_MIXER_READ_RECSRC */
	EREADSTEREO,	/* SOUND_MIXER_READ_STEREODEVS */
	EWRITERECSRC,	/* SOUND_MIXER_WRITE_RECSRC */
	EREADMIX,		/* MIXER_READ */
	EWRITEMIX,		/* MIXER_WRITE */
	ENOTOPEN,		/* mixer not open */
	EFILE			/* unable to open settings file */
};



/* name of mixer device file */
#define DEVICE_FILENAME "/dev/mixer"

int mixer_fd = -1;
int devmask = 0;
int recmask = 0;
int recsrc = 0;
int stereodevs = 0;


char *dev_name[SOUND_MIXER_NRDEVICES] = { \
	"Master", \
	"Bass", \
	"Treble", \
	"Synth", \
	"PCM", \
	"Speaker", \
	"Line", \
	"Mic", \
	"CD", \
	"Mix", \
	"PCM2", \
	"Record", \
	"Input", \
	"Output", \
	"Line 1", \
	"Line 2", \
	"Line 3", \
	"Digital1", \
	"Digital2", \
	"Digital3", \
	"Phone In", \
	"PhoneOut", \
	"Video", \
	"Radio", \
	"Monitor" };



