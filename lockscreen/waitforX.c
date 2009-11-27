/*

Copyright 1986, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

/*
 *      waitforX - wait for X server to start up
 *
 *	edited from xinit code by Curaga
 */

#include <unistd.h>
#include <stdio.h>
#include <X11/Xlib.h>

void usleep(unsigned long usec);

int main(int argc, char **argv) {

	int     cycles;		/* Wait cycle count */
	Display *xd=NULL;
	char *displayNum=":0";
	if (argc == 2) {
		displayNum=argv[1];
	}
	#ifdef DEBUG
	fprintf(stderr, "Opening DISPLAY '%s'\n",displayNum);
	#endif

	for (cycles = 0; cycles < 75; cycles++) { /* try up to 15 seconds */
		if ((xd = XOpenDisplay(displayNum))) {
			#ifdef DEBUG
			fprintf(stderr, "%d cycles done\n",cycles+1);
			#endif
			XCloseDisplay(xd);
			return 0;
		}
		usleep(200000); /* 1/5th of a sec */
	}

	/*fprintf (stderr, "Giving up.\n");*/
	return 1;
}
