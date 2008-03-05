# usplash-theme-debian - Makefile
#
# Copyright © 2006 - 2007 David Härdeman <david@hardeman.nu>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA



# This is the list of sizes to build and include in the theme
# By convention, the sizes should go from smaller to larger
sizes = 640x480 800x600 1024x768

# This creates a list with entries like background_800x600.png...
backgrounds = $(addsuffix .png, $(addprefix background_, $(sizes)))
background_objs = $(backgrounds:.png=.o)
progressfgs = $(addsuffix .png, $(addprefix progressfg_, $(sizes)))
progressfg_objs = $(progressfgs:.png=.o)
progressbgs = $(addsuffix .png, $(addprefix progressbg_, $(sizes)))
progressbg_objs = $(progressbgs:.png=.o)

CC       = gcc
CFLAGS   = -g -Wall -fPIC
LDFLAGS  =
INCLUDES =
COMPILE  = $(CC) $(INCLUDES) $(CFLAGS)
LINK     = $(CC) $(CFLAGS) $(LDFLAGS)

INSTALL      = install
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_PROG = $(INSTALL) -m 755

DESTDIR = /usr/lib/usplash

# Default = silent build
V =
ifeq ($(strip $(V)),)
E = @echo
Q = @
else
E = @\#
Q =
endif
export E Q


all: usplash-tcos.so
.PHONY: all
.DEFAULT: all

usplash-tcos.so: theme.o $(background_objs) $(progressfg_objs) $(progressbg_objs)
	$(E) "  LINK    " $@
	$(Q) $(COMPILE) -shared -o $@ $^

theme.h: theme.in progress.png themesetup.sh $(backgrounds) $(progressfgs) $(progressbgs)
	$(E) "  THMPARM " $@
	$(Q) ./themesetup.sh $(sizes) > $@

theme.o: theme.c theme.h

background_%.png: background.svg cmap.gif
	$(E) "  BG      " $@
	$(Q) rsvg-convert -w `echo $* | cut -dx -f1` -h `echo $* | cut -dx -f2` -f png $< > $@

progressfg_%.png: background_%.png progress.png cmap.gif
	$(E) "  PROGFG  " $@
	$(Q) composite -compose dst_over -gravity center $< progress.png $@

progressbg_%.png: background_%.png progress.png cmap.gif
	$(E) "  PROGBG  " $@
	$(Q) composite -gravity center -dissolve 25x100 progress.png $< png:- | \
	     composite -gravity center -compose src_over png:- progress.png $@

%.o: %.c
	$(E) "  CC      " $@
	$(Q) $(COMPILE) -o $@ -c $<

%.c: %.png cmap.gif
	$(E) "  PNGSRC  " $@
	$(Q) convert $< +dither -map cmap.gif gif:- | \
	     convert gif:- $<
	$(Q) pngtousplash $< > $@

#cmap.gif: background.svg progress.png
#	$(E) "  CMAP    " $@
#	$(Q) rsvg-convert -w 1024 -h 768 -f png background.svg | \
#	     composite -compose src_over -gravity center progress.png png:- +dither cmap.gif

install: usplash-tcos.so
	$(E) "  INSTALL " $<
	$(Q) $(INSTALL_DATA) -D $< $(DESTDIR)/usplash-tcos.so 
.PHONY: install

install-strip: usplash-tcos.so
	$(MAKE) INSTALL='$(INSTALL) -s' install
.PHONY: install-strip

clean:
	$(E) "  CLEAN   "
	$(Q) - find . -type f -name '*.png.c' -print0 | xargs -0r rm -f
	$(Q) - find . -type f -name '*.cfont' -print0 | xargs -0r rm -f
	$(Q) - find . -type f -name '*.o' -print0     | xargs -0r rm -f
	$(Q) - find . -type f -name '*.so' -print0    | xargs -0r rm -f
#	$(Q) - find . -type f -name 'cmap.*' -print0   | xargs -0r rm -f
#	$(Q) - find . -type f -name 'background_*' -print0   | xargs -0r rm -f
#	$(Q) - find . -type f -name 'progressfg_*' -print0   | xargs -0r rm -f
#	$(Q) - find . -type f -name 'progressbg_*' -print0   | xargs -0r rm -f

	$(Q) - find . -type f -name 'background_*.c' -print0   | xargs -0r rm -f
	$(Q) - find . -type f -name 'progressfg_*.c' -print0   | xargs -0r rm -f
	$(Q) - find . -type f -name 'progressbg_*.c' -print0   | xargs -0r rm -f
	$(Q) - rm -f theme.h
.PHONY: clean

.PRECIOUS: %.c progressbg_%.png progressfg_%.png background_%.png
