/* usplash
 *
 * Copyright © 2006 Canonical Ltd.
 * Copyright © 2006 Dennis Kaarsemaker <dennis@kaarsemaker.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef USPLASH_THEME_H
#define USPLASH_THEME_H

#include "bogl.h"

/* Current theme version */
#define THEME_VERSION 1

/* Theme structure definition */
struct usplash_theme {
	int version;			/* Always THEME_VERSION */

	struct bogl_pixmap *pixmap;	/* Background image */
	struct bogl_font   *font;	/* Font for writing text */

	/* Palette indexes */
	short background;		/* General background colour */
	short progressbar_background;	/* Colour of unreached progress */
	short progressbar_foreground;	/* Colour of current progress */
	short text_background;		/* Colour behind text */
	short text_foreground;		/* Normal colour of text */
	short text_success;		/* Colour of success highlight */
	short text_failure;		/* Colour of failure highlight */

	/* Progress bar position and size in pixels */
	short progressbar_x;
	short progressbar_y;
	short progressbar_width;
	short progressbar_height;

	/* Text box position and size in pixels */
	short text_x;
	short text_y;
	short text_width;
	short text_height;

	/* Text details */
	short line_height;		/* Height of line in pixels */
	short line_length;		/* Length of line in characters */
	short status_width;		/* Number of RHS pixels for status */
};

#endif /* USPLASH_THEME_H */
