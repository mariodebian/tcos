# vars



# functions used by tcos hooks

_echo () {
 # wrapper to echo in scripts, only print if debug is enabled
  if [ ${TCOS_DEBUG} ]; then
    echo "$@"
  fi
}

convert_links2files () {
# convert all links in $DESTDIR to linked files
# needed to calculate initramfs space
 #_echo "Converting links to files..."
 subdir=$1
 if [ "$subdir" = "" ]; then
   subdir="usr"
 fi
 dir=$(pwd)
 cd $DESTDIR
 # copy binary not links
 for xfile in $(find "$subdir" -type l); do
   if [ -f /$xfile ] ;then
     rm -f $DESTDIR/$xfile
     cp -f /$xfile $DESTDIR/$xfile
   else
     #_echo "    convert_links2files() "
     _link=$(readlink $xfile)
     #_echo "    link=$_link"
     rm -f $DESTDIR/$xfile
     cp -f $_link $DESTDIR/$xfile
   fi
 done
 cd $dir
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
  #diff_size=$(echo $((size2 -size1)) )
  diff_size=$(echo $size2 $size1 | awk '{print $1 - $2}' )
  if [ ${TCOS_DEBUG} ]; then
     _echo "  Package $1 got ${diff_size} Kb."
  fi
}

pathof() {
  # give absolute path of $1 binary (if found)
    location=$(which $1 | awk '{print $1}')
    if [ -z $location ]; then
        location=$(whereis $1 | awk '{print $2}')
    fi
    if [ ! -e $location ]; then
        echo "pathof() ERROR $1 not found !!!" 1>&2
    fi
    echo $location
}

cpifexists () {
 orig=$1
 dest=$2
  if [ -f $DESTDIR/$orig ]; then
    return 1
  fi
  
  
  if [ $# != 2 ]; then
   echo "    cpifexists(): ERROR: Need 2 arguments: \$1=$orig \$2=$dest"
   return 1
 fi
 
 if [ ! -f $orig ]; then
   echo "    cpifexits() WARNING: $orig not found, searching with pathof()..."
   _file=$(pathof $(basename $orig) )
   if [ "$_file" = "" ]; then
     echo "    cpifexists(): ERROR $orig not found in PATH, please install package that contain $orig."
     return 1
   elif [ -f "$_file" ]; then
     echo "    cpifexists(): FIXED: $(basename $orig) found in other path: $_file, using it!!!"
     orig=$_file
   else
     echo "   cpifexists(): ERROR: $(basename $orig) no found in PATH, please package that contain $orig"
     return 1
   fi   
 fi

 
 if [ ! -d $DESTDIR/$dest ]; then
   echo "  cpifexits() WARNING: $DESTDIR/$dest don't exists"
 fi


copy_exec "${orig}" "${dest}"
return 0
}

copydir () {
 if [ $# != 2 ]; then
   echo "copydir() ERROR Need 2 arguments: \$1=$1 \$2=$2"
   #_echo "     DEBUG: \$1=$1 \$2=$2"
   return 1
 fi
 if [ ! -d $1 ]; then
   echo "copydir() WARNING $1 dir not exits"
   #_echo "     DEBUG: \$1=$1 \$2=$2"
   return 1
 fi
 
#_echo "cp -ra \"${1}\" \"${DESTDIR}/${2}\""
cp -ra "${1}" "${DESTDIR}/${2}"
return 0
}

read_env_var() {
# read from env var
tmpvar=$(env| grep "^$1=" | awk -F "=" '{print $2}')
if [ "$tmpvar" = "" ]; then
  echo "$2"
else
  echo $tmpvar
fi
}
