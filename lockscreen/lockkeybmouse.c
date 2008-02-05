/*
* lockkeybmouse.c
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
#include <stdlib.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xmu/Converters.h>

#include<sys/stat.h>



#define AllPointerEventMask \
	(ButtonPressMask | ButtonReleaseMask | \
	EnterWindowMask | LeaveWindowMask | \
	PointerMotionMask | PointerMotionHintMask | \
	Button1MotionMask | Button2MotionMask | \
	Button3MotionMask | Button4MotionMask | \
	Button5MotionMask | ButtonMotionMask | \
	KeymapStateMask)

	
/* TODO: don't use X-Athena-Widgets (xaw)*/

int main (int argc, char **argv) {

	setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);

	XtResource desktopBackingStoreResources[] = { { XtNbackingStore, XtCBackingStore, XtRBackingStore, sizeof(int), 0, XtRImmediate, (XtPointer) Always, }, };

	Display * display;

	XSetWindowAttributes attr;
	unsigned long valuemask;

	Pixmap src, msk;
	XColor fg, bg;

	Widget toplevel;
	Dimension displayWidth, displayHeight;

	Cursor blankcursor;

	char srcBits[] = { 0,0,0,0,0 };
	char mskBits[] = { 0,0,0,0,0 };

    
	/* Call the main Xt initialisation function.  It parses command-line options, generating appropriate resource specs, and makes a
	 * connection to the X display. */
	toplevel = XtVaAppInitialize(NULL, "ScreenLocker", NULL, 0, &argc, argv, NULL, XtNborderWidth, 0, NULL);

	display = XtDisplay(toplevel);


	displayWidth = WidthOfScreen(DefaultScreenOfDisplay(display));
	displayHeight = HeightOfScreen(DefaultScreenOfDisplay(display));
	

	/* full screen - set position to 0,0, but defer size calculation until widgets are realized */
	XtVaSetValues(toplevel, XtNoverrideRedirect, True, XtNgeometry, "+0+0", NULL);


	

	XtVaSetValues(toplevel, XtNwidth, 1, XtNheight, 1, NULL);

	/* "Realize" all the widgets, i.e. actually create and map their X windows */
	XtRealizeWidget(toplevel);




	src = XCreateBitmapFromData(display, DefaultRootWindow(display), srcBits, 5, 5);
	msk = XCreateBitmapFromData(display, DefaultRootWindow(display), mskBits, 5, 5);
	

	
	
	
	blankcursor = XCreatePixmapCursor(display, src, msk, &fg, &bg, 2, 2);
	
	XFreePixmap(display, src);
	XFreePixmap(display, msk);

	XtVaGetApplicationResources(toplevel, (XtPointer)&attr.backing_store, desktopBackingStoreResources, 1, NULL);
	valuemask = CWBackingStore;

	attr.cursor = blankcursor;
	valuemask |= CWCursor;

  
	/* Try to get the input focus. */
	XSetInputFocus(display, DefaultRootWindow(display), RevertToPointerRoot, CurrentTime);

	/* now grab keyboard and mouse */
	if (XtGrabKeyboard(toplevel, True, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess ||
	    XtGrabPointer(toplevel, True, (unsigned int) AllPointerEventMask, GrabModeAsync, GrabModeAsync, None, blankcursor, CurrentTime) != GrabSuccess) {
		printf ("Could not grab keyboard and mouse.\n");
	}

#ifndef DEBUG
	while (1) {
	    /* wait in a loop */
		sleep (5);
	}
#else
    sleep(3);
    printf("DEBUG: quiting...\n");
#endif
	return (0);

}
