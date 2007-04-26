0008
TCOS Thin Client Operating System.
Available command line options

02 server=foo   07 IP address of XMDCP server (overwrite others)
02 volume=X%    07 set master and pcm volume levels to value (0%-100%)
02 zap          07 Allow to kill X with Ctrl + Alt + Backspace (default is not allowed)
02 startx=X     07 set default X session (R=remote L=local N=none)
02 res=800x600  07 set default X resolution (default 1024x768)
02 depth=16     07 set default X color depth (default 24 bits)
02 disablewheel 07 disable mouse wheel and 3 buttons (default enabled)
02 uselocal     07 force use of local hard disks (ext3 or vfat)
02 noswap       07 disable swap file creation (swap partition is used always)
02 force-update 07 Delete from terminal hard disk usr.squashfs downloaded in past session
02 nounionfs    07 Disable unionfs (requires 2 Mb of RAM)
02 noautofs     07 Disable autofs (automount daemon)
02 noltspfs     07 Disable ltsp filesystem (ltspfsd daemon)
02 opentelnet   07 write some debug entry's to /etc/inetd.conf
02 allmodules   07 Download allmodules.squashfs file (all kernel modules)
02 showmodules  07 Show loaded and discover modules
02 sshuser=XXX  07 User of SSHX
02 sshhost=XXX  07 Host of SSHX
02 sshport=XXX  07 ssh port of SSHX
02 sshdesktop=XXX 07 ssh desktop of SSHX (startkde, gnome-session, startxfce4)
02 break=XXX    07 Stop at breakpoint (see README file)

Press 08F1007 to return to main screen

