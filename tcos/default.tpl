default tcos
prompt 1
timeout 200
display __TFTP_MSG__
F0 __TFTP_MSG__
F1 __TFTP_MSG_HELP1__
F2 __TFTP_MSG_HELP2__

label tcos
  kernel vmlinuz-__TCOS_KERNEL__
  append ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash

label install
  kernel vmlinuz-__TCOS_KERNEL__
  append ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash startx=N installer

label update
  kernel vmlinuz-__TCOS_KERNEL__
  append ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash startx=N installer-update

label nfs
  kernel vmlinuz-__TCOS_KERNEL__
  append ramdisk_size=32768 initrd=initramfs-__TCOS_KERNEL__-nfs root=/dev/ram0 boot=tcos quiet splash


# other examples
#label tcos-low
#  kernel vmlinuz-__TCOS_KERNEL__
#  append ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet noautofs noltspfs nosound
#
