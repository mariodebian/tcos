# vars

# dir of download or used to mount througt NFS
download_dir=/mnt/tmp

# NFS server dir
nfs_dir=${TCOS_VAR}/tftp


###########################################
# functions in reboot and poweroff wrappers

wait_for() {
# enter in infinite loop waiting for file dir or anything
# $1 = file or dir
# $2 = [d f] type of $1
while [ ! -$2 $1 ]; do
     /sbin/udevtrigger
     sleep 1
done
}


start_usplash() {
  # kill usplash if cmdline have "nousplash" or "nosplash"
  for x in $(cat /proc/cmdline); do
        case $x in
        nosplash)
                return
                ;;
        nousplash)
                return
                ;;
        esac
  done

  if [ -x /sbin/usplash ]; then
        /sbin/usplash -c &
        /sbin/usplash_write "TEXT Starting usplash..."
        /sbin/usplash_write "TIMEOUT 180"
        /sbin/usplash_write "SUCCESS ok"
  fi
    if [ -x /sbin/splashy_update ]; then
        /sbin/splashy boot
        /sbin/splashy_update timeout180
  fi
}

kill_usplash() {
  if [ -x /sbin/usplash ]; then
    usplash_write "QUIT"  2> /dev/null
    killall usplash       2> /dev/null
    #chvt 1              2> /dev/null # this cause some problems :(
  fi
  if [ -x /sbin/splashy_update ]; then
    /sbin/splashy_update QUIT  2> /dev/null
    /sbin/splashy_update exit  2> /dev/null
    killall splashy     2> /dev/null
  fi
}

kill_xorg() {
  log_begin_msg "Killing Xorg"
    killall tryXorg >/dev/null 2>&1
    killall Xorg    >/dev/null 2>&1
  log_end_msg $?
  update_progress "-5"
}

kill_all() {
  # FIXME better scan ps output
  process="ltspfsd print_server pulseaudio ivs dhclient esd"
  for proc in ${process}; do
     log_begin_msg "Stopping ${proc}"
       killall $proc >  /dev/null 2>&1 &
     log_end_msg $?
     update_progress "-5"
  done
  # kill all with -9
  for proc in ${process}; do
     log_begin_msg "Force kill ${proc}"
       killall -9 $proc >  /dev/null 2>&1 &
     log_end_msg $?
     update_progress "-5"
  done
}

umount_swap() {
 log_begin_msg "Disable swap"
   swapoff -a
 log_end_msg $?
 update_progress "-5"
}

umount_all() {
 log_begin_msg "Umounting all"
   devices=$(grep -v ^none /proc/mounts | awk '{if ($2 != "" && $2 != "/dev" && $2 != "/dev/shm" && $2 != "/") print $2}')

   # Soft umount
   for dev in ${devices}; do
     #echo "Umount ${dev}"
     umount ${dev} >  /dev/null 2>&1
   done

   # Force umount
   for dev in ${devices}; do
     #echo "Force umount ${dev}"
     umount -l ${dev} >  /dev/null 2>&1
   done
  log_end_msg $?
  update_progress "-5"
}

# common functions used in tcos scripts


if_is_zero() {
 # $1 file
 if [ ! -f $1 ]; then
    _log "FUNCTIONS $1 no exists"
   echo 0
   return
 fi
 size=$(ls -l ${1} | awk '{print $5}')
 _log "FUNCTIONS size of $1 is ${size}"
 echo ${size}
}


_log () {
  /bin/logger -t "$(echo $1| awk '{print $1}')" "$@" >> /tmp/initramfs.debug 2>&1
}

read_server() {
  # $1 is server hostname
  nSERVER=$(grep "$1" /etc/hosts | awk '{print $1}' | head -1)
  if [ "${nSERVER}" = "" ]; then
    nSERVER=$(get_server)
  fi
  echo "${nSERVER}"
}


get_server() {
  if [ ${TCOS_FORCE_SERVER} ]; then
   echo ${TCOS_FORCE_SERVER}
   return
  fi
  # read server ip address from dhcp
  if [ ! -e /var/lib/dhcp/dhclient.leases ] || [ $(cat /var/lib/dhcp/dhclient.leases | wc -l) = 0 ]; then
    clear
    panic "Error, network not configured, check your DHCP server conf."
  fi
  SERVER=${TCOS_SERVER}
  SERVER=$(grep dhcp-server /var/lib/dhcp/dhclient.leases | awk '{print $3}' | awk -F ";" '{print $1}')
  # overwrite with cmdline
  # DOCUMENTME server | ip of XDMCP server
  SERVER=$(read_cmdline_var "server" "${SERVER}")
  echo "${SERVER}"
}

download_file () {
# $1 remote file
# $2 local file
mkdir $(dirname $2) >/dev/null 2>&1
_log "tftp -g -r ${1} -l ${2} "$(read_server "tftp-server")
rm -f /tmp/downloading
touch /tmp/downloading
/sbin/down-listener &
tftp -g -r ${1} -l ${2} $(read_server "tftp-server") > /dev/null 2> /tmp/download_file.log
rm -f /tmp/downloading
if [ $? = 0 ] ;then
 _log "download_file() OK"
 return 0
else
 _log "download_file() Error"
 cat /tmp/download_file.log >> /tmp/initramfs.debug 2> /dev/null
 rm /tmp/download_file.log > /dev/null 2>&1
 return 1
fi
}

read_cmdline_var() {
 # read cmdline and print to stdout var value if found
 # $1 var name
 # $2 var default value
 unset var
 for x in $(cat /proc/cmdline); do
        case $x in
        $1=*)
                var=${x#$1=}
                ;;
        $1)
                var=1
                ;;
        esac
 done
 if [ ! -z ${var} ]; then
  echo "${var}"
  _log "read_cmdline() reading $1 cmdline=${var}"
 else
  echo "${2}"
  _log "read_cmdline() reading $1 default=${2}"
 fi
}


update_progress() {
  # read /dev/.initramfs/progress_state and update
  # $1 is number that sum at counter
  sum=${1}
  if [ "${sum}" = "" ]; then
    # increase progressbar step
    sum=3
  fi
  # /tmp/progress is created in scripts/tcos-top/10foo with value=5
  old=$(cat /tmp/progress)
  new=$(echo $old $sum | awk '{print $1+$2}')
  #new=$((${old}+${sum}))
  if [ -x /sbin/usplash_write ]; then
    /sbin/usplash_write "PROGRESS ${new}"
    #_log "updating progress to ${new} %"
    echo ${new} > /tmp/progress
  fi
  if [ -x /sbin/splashy_update ]; then
    /sbin/splashy_update "progress${new}"
    #_log "updating progress to ${new} %"
    echo ${new} > /tmp/progress
  fi
}




stat_before () {
  # read space in $DESTDIR (this functions is a checkpoint)
  size1=$(du -s ${DESTDIR} 2>/dev/null| awk '{print $1}')
}
stat_after () {
  # read size after checkpoint and prints diff between disk space
  # this give what space need an app
  convert_links2files
  size2=$(du -s ${DESTDIR} 2>/dev/null| awk '{print $1}')
  diff_size=$(echo $((size2 -size1)) )
  if [ ${TCOS_DEBUG} ]; then
     _echo "Package $1 get ${diff_size} Kb."
  fi
}

mount_aufs() {
  ramdisk=$1
  rofs=$2
  rwfs=$3
  #
  # example:
  # mount_aufs /mnt/ram   /.usr /usr
  #               RAM      RO    RW
  #
  _log "AUFS Creating ramdisk ${ramdisk} of 2 Mb"
   mkdir -p ${ramdisk} >> /tmp/initramfs.debug 2>&1
   mount -t tmpfs -o "size=2m" tmpfs ${ramdisk} >> /tmp/initramfs.debug 2>&1

   _log "AUFS Moving ${rwfs} squashfs to ${rofs}"
   # move /usr
   mkdir -p ${rofs} >> /tmp/initramfs.debug 2>&1
   mount -o move ${rwfs} ${rofs}

   _log "AUFS Mount with aufs ${rofs} and ${ramdisk} to create ${rwfs} in rw mode"
   # mount aufs
   mount -t aufs -o br:${ramdisk}:${rofs} none ${rwfs} >> /tmp/initramfs.debug 2>&1
}

mount_unionfs() {
  # DOCUMENTME nounionfs | disable unionfs from /usr mount point
  nounionfs=$(read_cmdline_var "nounionfs" "0")
  if [ ${nounionfs} = 1 ]; then
     _log "UNIONFS disabled from cmdline"
     return
  fi
  # if module not loaded try with aufs or exit :(
  if [ $(grep unionfs /proc/modules| wc -l) = 0 ]; then
    if [ $(grep aufs /proc/modules| wc -l) != 0 ]; then
      mount_aufs $1 $2 $3
      return
    fi
    _log "UNIONFS ERROR mounting unionfs or aufs in rw mode"
    return
  fi
  # mount_unionfs()
  # remount $3 filesystem in rw mode
  # $1 ramdisk
  # $2 ro filesystem
  # $3 union
  # $3 contains a mounted filesystem in ro mode (squashfs)
  ramdisk=$1
  rofs=$2
  rwfs=$3
  #
  # example:
  # mount_unionfs /mnt/ram /.usr /usr
  #               RAM      RO    RW
  #
  _log "UNIONFS Creating ramdisk ${ramdisk} of 2 Mb"
   mkdir -p ${ramdisk} >> /tmp/initramfs.debug 2>&1
   # not needed because / is a big ramdisk
   mount -t tmpfs -o "size=2m" tmpfs ${ramdisk} >> /tmp/initramfs.debug 2>&1

   _log "UNIONFS Moving ${rwfs} squashfs to ${rofs}"
   # move /usr
   mkdir -p ${rofs} >> /tmp/initramfs.debug 2>&1
   mount -o move ${rwfs} ${rofs}

   _log "UNIONFS Mount with unionfs ${rofs} and ${ramdisk} to create ${rwfs} in rw mode"
   # mount union
   mount -t unionfs -o dirs=${ramdisk}=rw:${rofs}=ro unionfs ${rwfs} >> /tmp/initramfs.debug 2>&1
}


