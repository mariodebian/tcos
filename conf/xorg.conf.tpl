# xorg.conf (Xorg X Window System server configuration file)
#
# This file was generated by dexconf, the Debian X Configuration tool, using
# values from the debconf database.
#
# Edit this file with caution, and see the xorg.conf manual page.
# (Type "man xorg.conf" at the shell prompt.)
#
# This file is automatically updated on xserver-xorg package upgrades *only*
# if it has not been modified since the last upgrade of the xserver-xorg
# package.
#
#    FILE generated by configurexorg at __date__
#
# If you have edited this file but would like it to be automatically updated
# again, run the following commands as root:
#
#   cp /etc/X11/xorg.conf /etc/X11/xorg.conf.custom
#   md5sum /etc/X11/xorg.conf >/var/lib/xfree86/xorg.conf.md5sum
#   dpkg-reconfigure xserver-xorg

Section "Files"
	FontPath	"unix/:7100"			# local font server
	# if the local font server has problems, we can fall back on these
__xlocalfonts__	FontPath	"/usr/lib/X11/fonts/misc"
__xlocalfonts__	FontPath	"/usr/lib/X11/fonts/cyrillic"
__xlocalfonts__	FontPath	"/usr/lib/X11/fonts/100dpi/:unscaled"
__xlocalfonts__	FontPath	"/usr/lib/X11/fonts/75dpi/:unscaled"
__xlocalfonts__	FontPath	"/usr/lib/X11/fonts/Type1"
__xlocalfonts__	FontPath	"/usr/lib/X11/fonts/CID"
__xlocalfonts__	FontPath	"/usr/lib/X11/fonts/100dpi"
__xlocalfonts__	FontPath	"/usr/lib/X11/fonts/75dpi"
EndSection

Section "Module"
	Load	"bitmap"
	Load	"dbe"
	Load	"ddc"
	Load	"dri"
	Load	"extmod"
	Load	"freetype"
	Load	"glx"
	Load	"int10"
	Load	"record"
	Load	"type1"
	Load	"v4l"
	Load	"vbe"
EndSection

Section "InputDevice"
	Identifier	"Generic Keyboard"
	Driver		"keyboard"
	Option		"CoreKeyboard"
	Option		"XkbRules"	"xorg"
	Option		"XkbModel"	"pc105"
	Option		"XkbLayout"	"es"
EndSection

Section "InputDevice"
	Identifier	"Serial Mouse0"
	Driver		"mouse"
	Option		"SendCoreEvents"
	Option		"Device"		"/dev/ttyS0"
	Option		"Protocol"		"auto"
	Option		"Emulate3Buttons"	"true" 
EndSection

Section "InputDevice"
	Identifier	"Serial Mouse1"
	Driver		"mouse"
	Option		"SendCoreEvents"
	Option		"Device"		"/dev/ttyS1"
	Option		"Protocol"		"auto"
	Option		"Emulate3Buttons"	"true" 
EndSection

Section "InputDevice"
	Identifier	"Configured Mouse"
	Driver		"mouse"
	Option		"CorePointer"
	Option		"Device"		"__xmousedev__"
__xmousenowheel__	Option		"Protocol"		"__xmouseprotocol__"
__xmousewheel__	Option		"Protocol"		"__xmouseprotocol__"
__xmousewheel__	Option		"Emulate3Buttons"	"true" 
__xmousewheel__	Option		"ZAxisMapping"		"4 5"
EndSection

Section "Device"
	Identifier	"Generic Video Card"
	Driver		"__xdriver__"
__xdriver_via__        Option          "EnableAGPDMA"
__xdriver_via__        Option          "DisableIRQ"
__xdriver_via__        Option          "VBEModes" "true"
__xdriver_via__        Option          "VBERestore"    "true"

EndSection

Section "Monitor"
	Identifier	"Generic Monitor"
__xdpms__	Option		"DPMS"
	HorizSync	__xhorizsync__
	VertRefresh	__xvertsync__
EndSection

Section "Screen"
	Identifier	"Default Screen"
	Device		"Generic Video Card"
	Monitor		"Generic Monitor"
	DefaultDepth	__xdepth__
	SubSection "Display"
		Depth		__xdepth__
		Modes		"__xres__" "1024x768" "" "800x600" "" "640x480" 
	EndSubSection
EndSection

Section "ServerLayout"
	Identifier	"Default Layout"
	Screen		"Default Screen"
	InputDevice	"Generic Keyboard"
	InputDevice	"Configured Mouse"
	InputDevice	"Serial Mouse0"
	InputDevice	"Serial Mouse1"
EndSection

Section "DRI"
	Mode	0666
EndSection

__dontzap__

#xdriver=__xdriver__
#xres=__xres__
#xdepth=__xdepth__
#xhorizsync=__xhorizsync__
#xvertsync=__xvertsync__
#xrefresh=__xrefresh__
#xmousedev=__xmousedev__
#xmouseprotocol=__xmouseprotocol__
#xmousenowheel=__xmousenowheel__
#xmousewheel=__xmousewheel__
#xdpms=__xdpms__
#xlocalfonts=__xlocalfonts__
#xfontserver=__xfontserver__
#xdontzap=__xdontzap__
#xdriver_via=__xdriver_via__
