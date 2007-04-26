# common file of shfsmount y shfsumount scripts

mountpoint=${HOME}/Desktop/Devices
myhost=$(echo $DISPLAY | cut -d: -f1)

if [ -n "${myhost}" ]; then
 myremoteip=$(grep -i ${myhost} /etc/hosts| cut -f1)
fi

is_mounted() {
 # check if mountpoint is mounted
 ismounted=$(grep ${mountpoint} /proc/mounts | wc -l)
 if [ ${ismounted} -gt 0 ]; then
  echo 0
 else
  echo 1
 fi
}

is_remote() {
 # return 1 if remote return 0 localhost
 if [ -n "${myhost}" -a "${myhost}" != "localhost" ]; then
  echo 0
 else
  echo 1
 fi
}
