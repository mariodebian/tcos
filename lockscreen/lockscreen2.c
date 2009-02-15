/*
* lockscreen2.c
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
#include <X11/Xlib.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xmu/Converters.h>
#include "X11/extensions/shape.h"

#include <Imlib2.h>

#ifndef LOCKSCREEN_IMAGE
#define LOCKSCREEN_IMAGE "/usr/share/tcos-core/lockscreen-custom.png"
#endif

#ifndef LOCKSCREEN_IMAGE_TCOS
#define LOCKSCREEN_IMAGE_TCOS "/usr/share/tcos-core/lockscreen.png"
#endif


int setenv(const char *name, const char *value, int overwrite);

#include<sys/stat.h>

int file_exists (char * fileName)
{
    struct stat buf;
    int i = stat ( fileName, &buf );
    /* File found */
    if ( i == 0 )
        return 1;
    
    return 0;
}



#define AllPointerEventMask \
	(ButtonPressMask | ButtonReleaseMask | \
	EnterWindowMask | LeaveWindowMask | \
	PointerMotionMask | PointerMotionHintMask | \
	Button1MotionMask | Button2MotionMask | \
	Button3MotionMask | Button4MotionMask | \
	Button5MotionMask | ButtonMotionMask | \
	KeymapStateMask)


int main (int argc, char **argv) {
	XtResource desktopBackingStoreResources[] = { { XtNbackingStore, XtCBackingStore, XtRBackingStore, sizeof(int), 0, XtRImmediate, (XtPointer) Always, }, };

	Display * display;
	Visual * vis;

	XSetWindowAttributes attr;
	unsigned long valuemask;
	int count = 0;
	Pixmap src, msk;
	XColor fg, bg;
	int blackColor;
	GC gc;

	Widget toplevel;
	Dimension displayWidth, displayHeight;

	Cursor blankcursor;

	Widget form, viewport, desktop;
	Window desktop_win;

	char srcBits[] = { 0,0,0,0,0 };
	char mskBits[] = { 0,0,0,0,0 };

    Imlib_Image *image;
    Pixmap pixmap, mask;
    int image_x, image_y, rc;
    int screen, depth;
    
    setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);

	/* Call the main Xt initialisation function.  It parses command-line options, generating appropriate resource specs, and makes a
	 * connection to the X display. */
	toplevel = XtVaAppInitialize(NULL, "ScreenLocker", NULL, 0, &argc, argv, NULL, XtNborderWidth, 0, NULL);

	display = XtDisplay(toplevel);

	vis = DefaultVisual(display, DefaultScreen(display));

	displayWidth = WidthOfScreen(DefaultScreenOfDisplay(display));
	displayHeight = HeightOfScreen(DefaultScreenOfDisplay(display));
	screen = DefaultScreen(display);
	depth =  DefaultDepth(display, screen);

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

	/* Now we want to fix the size of "viewport".  We shouldn't just change it directly.
	 * Instead we set "toplevel" to the required size (which should propagate through "form" to "viewport").
	 * Then we remove "viewport" from being managed by "form", change its resources to position it and make sure that "form" won't
	 * attempt to resize it, then ask "form" to manage it again. */
	XtResizeWidget(toplevel, displayWidth, displayHeight, 0);
	XtUnmanageChild(viewport);
	XtVaSetValues(viewport, XtNhorizDistance, 0, XtNvertDistance, 0, XtNleft, XtChainLeft, XtNright, XtChainLeft, XtNtop, XtChainTop, XtNbottom, XtChainTop, NULL);
	XtManageChild(viewport);

	/* Now we can set "toplevel" to its proper size. */
	XtResizeWidget(toplevel, displayWidth, displayHeight, 0);

	/*image = XCreateImage(display, vis, 32, ZPixmap, 0, NULL, 2000, 2000, BitmapPad(display), 0);*/
	blackColor = BlackPixel(display, DefaultScreen(display));
	desktop_win = XtWindow(desktop);
	gc = XCreateGC(display, desktop_win, 0, NULL);


    if ( file_exists (LOCKSCREEN_IMAGE) || file_exists(LOCKSCREEN_IMAGE_TCOS) )
    {
        imlib_context_set_display(display);
        imlib_context_set_visual(DefaultVisual(display, DefaultScreen(display)));
        imlib_context_set_colormap(DefaultColormap(display, DefaultScreen(display)));
        imlib_set_color_usage(128);
        imlib_context_set_dither(1);
        
        if ( file_exists(LOCKSCREEN_IMAGE) ) {
            printf("loading custom image %s...\n", LOCKSCREEN_IMAGE);
            image=imlib_load_image(LOCKSCREEN_IMAGE);
        }
        else {
            printf("loading TCOS image %s...\n", LOCKSCREEN_IMAGE_TCOS);
            image=imlib_load_image(LOCKSCREEN_IMAGE_TCOS);
        }
        
        imlib_context_set_image(image);
        image_x = imlib_image_get_width();
        image_y = imlib_image_get_height();
        printf("image x=%d y=%d displayWidth=%d displayHeight=%d\n", image_x, image_y, displayWidth, displayHeight);


        pixmap = XCreatePixmap(display, desktop_win, displayWidth, displayHeight, depth);
        
        imlib_context_set_drawable(desktop_win);
        imlib_render_image_on_drawable_at_size(0, 0, displayWidth, displayHeight);
        imlib_render_pixmaps_for_whole_image_at_size(&pixmap, &mask, displayWidth, displayHeight);
        
        rc = XSetWindowBackgroundPixmap(display, desktop_win, pixmap );
        switch (rc) {
            case BadMatch:
                fprintf(stderr, "XSetWindowBackgroundPixmap - BadMatch.\n");
                exit(1);
                break;
            case BadPixmap:
                fprintf(stderr,
                        "XSetWindowBackgroundPixmap - BadPixmap.\n");
                exit(1);
                break;
            case BadWindow:
                fprintf(stderr, "XSetWindowBackgroundPixmap - BadWindow.\n");
                exit(1);
                break;
        }

        
        if (mask) {
            XShapeCombineMask(display, desktop_win,
                            ShapeBounding, 0, 0, mask, ShapeSet);
        }
        
        
        XMapWindow(display, desktop_win);
        XSetForeground(display, gc, blackColor);
        XFillRectangle( display, pixmap, gc, 0, 0, displayWidth, displayHeight );
        
        if (image)
            imlib_free_image();

        if(pixmap)
            XFreePixmap(display, pixmap);

        if(pixmap)
            imlib_free_pixmap_and_mask(pixmap);

        pixmap = None;

        if(gc)
            XFreeGC(display, gc);
    }

    else {
        /* Tell the GC we draw using the black color*/
        printf("file %s not exists!!\n", LOCKSCREEN_IMAGE);
        XSetForeground(display, gc, blackColor);
	    XFillRectangle(display, desktop_win, gc, 0, 0, displayWidth, displayHeight);
    }



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
		printf ("lockscreen::screensaver Could not disable screensaver, Badvalue=%d Count=%d.\n", BadValue, count);
		sleep (1);
		count++;
	}

	XUngrabKeyboard(display, CurrentTime);
	XUngrabPointer(display, CurrentTime);

	count=0;
	/* now grab keyboard and mouse */
	while (count < 5) {
		if (XtGrabKeyboard(toplevel, True, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess ||
	    	XtGrabPointer(toplevel, True, (unsigned int) AllPointerEventMask, GrabModeAsync, GrabModeAsync, None, 			blankcursor, CurrentTime) != GrabSuccess) {
			printf ("lockscreen::keybmouse Could not grab keyboard and mouse, Count=%d.\n", count);
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
	XCloseDisplay(display);
	return(0);

}
