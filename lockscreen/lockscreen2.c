/*# lockscreen2.c standalone Xorg screen locker C 2007-06-06 14:22:40 mariodebian $
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
#include <X11/Shell.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xmu/Converters.h>

#include <Imlib.h>

#ifndef LOCKSCREEN_IMAGE
#define LOCKSCREEN_IMAGE "/usr/share/initramfs-tools-tcos/xmlrpc/lockscreen.png"
#endif


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

	
/* TODO: don't use X-Athena-Widgets (xaw)*/

int main (int argc, char **argv) {

	XtResource desktopBackingStoreResources[] = { { XtNbackingStore, XtCBackingStore, XtRBackingStore, sizeof(int), 0, XtRImmediate, (XtPointer) Always, }, };

	Display * display;
	Visual * vis;

	XSetWindowAttributes attr;
	unsigned long valuemask;

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

    ImlibData *id;
    ImlibImage *image;
    Pixmap pixmap, mask;
    int image_x, image_y, rc;
    
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


    if ( file_exists (LOCKSCREEN_IMAGE) )
    {
        printf("loading image %s...\n", LOCKSCREEN_IMAGE);
        id = Imlib_init(display);
        image=Imlib_load_image(id,LOCKSCREEN_IMAGE);
        image_x = image->rgb_width;
        image_y = image->rgb_height;

        Imlib_apply_image(id, image, desktop_win);


        pixmap = Imlib_move_image( id,image );
        mask = Imlib_move_mask( id, image );

        

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
        XFillRectangle( display, pixmap, gc, 0, 0, image_x, image_y );
        XFreePixmap(display, pixmap);
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

	/* now grab keyboard and mouse */
	if (XtGrabKeyboard(desktop, True, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess ||
	    XtGrabPointer(desktop, True, (unsigned int) AllPointerEventMask, GrabModeAsync, GrabModeAsync, None, blankcursor, CurrentTime) != GrabSuccess) {
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
