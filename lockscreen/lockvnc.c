/*
* lockvnc.c
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
#include <stdlib.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xmu/Converters.h>

#include<sys/stat.h>

int setenv(const char *name, const char *value, int overwrite);

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

	XtResource desktopBackingStoreResources[] = { { XtNbackingStore, XtCBackingStore, XtRBackingStore, sizeof(int), 0, XtRImmediate, (XtPointer) Always, }, };

	Display * display;
	Visual * vis;

	XSetWindowAttributes attr;
	unsigned long valuemask;
	int count = 0;
	Pixmap src, msk;
	XColor fg, bg;

	Widget toplevel;
	Dimension displayWidth, displayHeight;

	Cursor blankcursor;

	Widget form, viewport, desktop;
	Window desktop_win;

	char srcBits[] = { 0,0,0,0,0 };
	char mskBits[] = { 0,0,0,0,0 };

    setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);

	/* Call the main Xt initialisation function.  It parses command-line options, generating appropriate resource specs, and makes a
	 * connection to the X display. */
	toplevel = XtVaAppInitialize(NULL, "ScreenLocker", NULL, 0, &argc, argv, NULL, XtNborderWidth, 0, NULL);

	display = XtDisplay(toplevel);

	vis = DefaultVisual(display, DefaultScreen(display));

	displayWidth = WidthOfScreen(DefaultScreenOfDisplay(display));
	displayHeight = HeightOfScreen(DefaultScreenOfDisplay(display));
	

	/* full screen - set position to 0,0, but defer size calculation until widgets are realized */
	XtVaSetValues(toplevel, XtNoverrideRedirect, True, XtNgeometry, "+0+0", NULL);


	form = XtVaCreateManagedWidget("form", formWidgetClass, toplevel, XtNborderWidth, 0, XtNdefaultDistance, 0, NULL);
	viewport = XtVaCreateManagedWidget("viewport", viewportWidgetClass, form, XtNborderWidth, 0, NULL);
	desktop = XtVaCreateManagedWidget("desktop", coreWidgetClass, viewport, XtNborderWidth, 0, NULL);

	XtVaSetValues(desktop, XtNwidth, displayWidth, XtNheight, displayHeight, NULL);

	/* "Realize" all the widgets, i.e. actually create and map their X windows */
	XtRealizeWidget(toplevel);



	/* We want to stop the window manager from managing our toplevel window. This is not really a nice thing to do, so may not work
	 * properly with every window manager.  We do this simply by setting overrideRedirect and reparenting our window to the root. 
	 * The window manager will get a ReparentNotify and hopefully clean up its frame window. */
	XtVaSetValues(toplevel, XtNoverrideRedirect, True, NULL);
	XReparentWindow(display, XtWindow(toplevel), DefaultRootWindow(display), 0, 0);

	 /* attempt to resize it, then ask "form" to manage it again. */
	XtResizeWidget(toplevel, 1, 1, 0);

	desktop_win = XtWindow(desktop);


	src = XCreateBitmapFromData(display, DefaultRootWindow(display), srcBits, 5, 5);
	msk = XCreateBitmapFromData(display, DefaultRootWindow(display), mskBits, 5, 5);
	
	
	XAllocNamedColor(display, DefaultColormap(display,DefaultScreen(display)), "black", &fg, &fg);
	XAllocNamedColor(display, DefaultColormap(display,DefaultScreen(display)), "white", &bg, &bg);
	
	
	
	blankcursor = XCreatePixmapCursor(display, src, msk, &fg, &bg, 2, 2);
	
	XFreePixmap(display, src);
	XFreePixmap(display, msk);

	XtVaGetApplicationResources(desktop, (XtPointer)&attr.backing_store, desktopBackingStoreResources, 1, NULL);
	valuemask = CWBackingStore;

	attr.cursor = blankcursor;
	valuemask |= CWCursor;

	XChangeWindowAttributes(display, desktop_win, valuemask, &attr);
  
	/* Try to get the input focus. */
	XSetInputFocus(display, DefaultRootWindow(display), RevertToPointerRoot, CurrentTime);

	/* Try to dont allow screensaver to activate */
	XForceScreenSaver(display, (int) ScreenSaverReset);
	while (count < 5) {
		if (XSetScreenSaver(display, 0, 0, (int) DontPreferBlanking, (int) DontAllowExposures) != BadValue ) {
			break;
		}
		printf ("lockvnc::screensaver Could not disable screensaver, Badvalue=%d Count=%d.\n", BadValue, count);
		sleep (1);
		count++;
	}

	/*XUngrabKeyboard(display, CurrentTime);
	XUngrabPointer(display, CurrentTime);*/

	count=0;
	/* now grab keyboard and mouse */
	while (count < 5) {
		if (XtGrabKeyboard(toplevel, True, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess) {
			printf ("lockvnc::keybmouse Could not grab keyboard, Count=%d.\n", count);
			sleep (1);
			count++;
		}
		else {
			break;
		}
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
