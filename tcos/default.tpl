default tcos
prompt 1
timeout 200
display tcos.msg
F0 tcos.msg
F1 help.msg
F2 help2.msg

label tcos
  kernel vmlinuz-__TCOS_KERNEL__
  append ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash

label update
  kernel vmlinuz-__TCOS_KERNEL__
  append ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet splash installer-update

label nfs
  kernel vmlinuz-__TCOS_KERNEL__
  append ramdisk_size=32768 initrd=initramfs-__TCOS_KERNEL__-nfs root=/dev/ram0 boot=tcos quiet splash


# other examples
#label tcos-low
#  kernel vmlinuz-__TCOS_KERNEL__
#  append ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet discover=0 noautofs noltspfs nosound
#
#label tcos-new-pc
#  kernel vmlinuz-__TCOS_KERNEL__
#  append ramdisk_size=65536 initrd=initramfs-__TCOS_KERNEL__ root=/dev/ram0 boot=tcos quiet showmodules discover=1
