/*
* screensize.c: returns scren size in horiz x vert
* Copyright (C) 2006,2007,2008  mariodebian at gmail
* Copyright (C) 2008  vidal_joshur at gva.es
*
*  This file is based on  screen-locker of iTALC project:
*   screen_locker.c - screen-locker, which is part of the iTALC client-software
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



#include <stdio.h>
#include <unistd.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>


int main (int argc, char **argv) {

	Display * display;

	Widget toplevel;
	Dimension displayWidth, displayHeight;


    
	/* Call the main Xt initialisation function.  It parses command-line options, generating appropriate resource specs, and makes a
	 * connection to the X display. */
	toplevel = XtVaAppInitialize(NULL, "ScreenLocker", NULL, 0, &argc, argv, NULL, XtNborderWidth, 0, NULL);

	display = XtDisplay(toplevel);


	displayWidth = WidthOfScreen(DefaultScreenOfDisplay(display));
	displayHeight = HeightOfScreen(DefaultScreenOfDisplay(display));
	
	printf("%dx%d\n", displayWidth, displayHeight);

	XCloseDisplay(display);
	return (0);

}
