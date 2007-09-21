#!/bin/sh

_file=/tmp/tcosxmlrpc.dat
log_file=/var/log/exejobs.log

mkdir -p /var/log
echo "exejobs started at: $(date)" >> ${log_file}

touch ${_file}
chmod 666 ${_file}
timeout=2

delete_proc() {
lines=$(cat $_file| wc -l)
line1=$(($lines-1))
tail -${line1} ${_file} > ${_file}.new
mv ${_file}.new ${_file}
}

read_proc() {
proc=$(head -1 ${_file} |tail -1)
if [ "${proc}" != "" ]; then
 echo "$(date) exejobs::read_proc() => ${proc}"
 $(${proc}) >> ${log_file} 2>&1 &
 delete_proc
fi
}



while [ 1 ]; do
 sleep ${timeout}
 read_proc
done


