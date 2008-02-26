DEFAULT __TCOS_MENU_TYPE__
MENU BACKGROUND pxetcos.jpg
PROMPT 0
TIMEOUT 100
ONTIMEOUT tcos

#NOESCAPE 1
#ALLOWOPTIONS 1
#MENU MASTER PASSWD __TCOS_MENU_PASSWD__

MENU TITLE Choose a network boot option....

label tcos
  MENU DEFAULT
  MENU LABEL ^TCOS default ( RAM > 40 Mb)
  KERNEL vmlinuz-__TCOS_KERNEL__
  APPEND ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash

label nfs
  MENU LABEL TCOS ^NFS     ( RAM < 40 Mb)
  KERNEL vmlinuz-__TCOS_KERNEL__
  APPEND ramdisk_size=32768 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash

label install
  __TCOS_NETBOOT_HIDE_INSTALL__MENU HIDE
  MENU LABEL TCOS ( ^install images on HDD)
  KERNEL vmlinuz-__TCOS_KERNEL__
  APPEND ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash startx=N installer

label update
  __TCOS_NETBOOT_HIDE_INSTALL__MENU HIDE
  MENU LABEL TCOS ( ^update installed images on HDD)
  KERNEL vmlinuz-__TCOS_KERNEL__
  APPEND ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash startx=N installer-update



