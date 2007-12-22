/*# screensize.c standalone Xorg screen size C 2007-06-06 14:22:40 mariodebian and ignacio vidal $
#
# This file is part of tcosxmlrpc.
#
#  This file is based on  screen-locker of iTALC project:
#   screen_locker.c - screen-locker, which is part of the iTALC client-software
#
#   iTALC/Client-software
#   Copyright (c) 2004-2005 Tobias Doerffel <tobias@doerffel.de>
#
#
# tcosxmlrpc is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# tcosxmlrpc is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with tcosxmlrpc; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.
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

	return (0);

}
