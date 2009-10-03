# vars



# functions used by tcos hooks

_ldd () {
  # wrapper to ldd
  LC_ALL=C ldd $@ 2>/dev/null || return 1
  return 0
}

_echo () {
 # wrapper to echo in scripts
    echo "$@"
}

_debug() {
  if [ ${TCOS_DEBUG} ]; then
    echo "   DEBUG :: $@" >&2
  fi
}

_verbose() {
  if [ ${TCOS_VERBOSE} ]; then
    echo "## VERBOSE ## '$@'" >&2
  fi
}

_warning() {
  # always show
  echo "## WARNING ## '$@'" >&2
}



read_template() {
  [ ! -e "$1" ] && return
  tpl=$(awk -F"=" '/^TCOS_TEMPLATE=/ {print $2}' $1)
  if [ "$tpl" = "" ]; then
    _verbose "(t-g-f) Not found TEMPLATE in $1, searching BASED_TEMPLATE"
    tpl=$(awk -F"=" '/^TCOS_BASED_TEMPLATE=/ {print $2}' $1)
  fi
  [ "$tpl" = "" ] && _warning "(t-g-f) template don't have TCOS_TEMPLATE or TCOS_BASED_TEMPLATE. BUG???" && return
  #_debug "Template $tpl"
  for tdir in /usr/share/initramfs-tools-tcos/templates /etc/tcos/templates; do
    _verbose "(t-g-f) Searching in dir $tdir"
    [ -f $tdir/$tpl ] && echo "$tdir/$tpl" && break
  done
}

tcos_get_templates() {
  # read template from tcos.conf
  tpl1=$(read_template /etc/tcos/tcos.conf)
  tpl2=""
  personalized=""
  _verbose "(t-g-f) Default template $tpl"
  for tfile in $(find /etc/tcos/templates -type f -name "*.conf"); do
    if [ -f $tfile ]; then
      #_debug "$tfile file exists, adding to tpl2"
      tpl2="$tpl2 $(read_template $tfile)"
      personalized=$tfile
    else
      _verbose "(t-g-f) Based template $tfile not found, ignoring"
    fi
  done
  if [ "$( echo $tpl2 | sed 's/[[:blank:]]//g' )" != "" ]; then
    _verbose "(t-g-f) return tpl2='base.conf $tpl2 $personalized'"
    echo "/usr/share/initramfs-tools-tcos/templates/base.conf $tpl2 $personalized"
  else
    _verbose "(t-g-f) return tpl1='base.conf $tpl1'"
    echo "/usr/share/initramfs-tools-tcos/templates/base.conf $tpl1"
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
   _link=$(readlink $xfile)
   if [ $(echo $_link | grep -c ${TCOS_PKG_CACHE}) != 0 ]; then
     rm -f $DESTDIR/$xfile
     cp -f $_link $DESTDIR/$xfile
     #_echo " ******** convert_link2files() CACHED FILE => cp -f $_link $DESTDIR/$xfile"

   elif [ -f /$xfile ] ;then
     rm -f $DESTDIR/$xfile
     cp -f /$xfile $DESTDIR/$xfile
     #_echo "  :::::::  convert_link2files() :::::A:::: cp /$xfile $DESTDIR/$xfile"

   else
     #_echo "    convert_links2files() "
     rm -f $DESTDIR/$xfile
     cp -f $_link $DESTDIR/$xfile
     #_echo "  :::::::  convert_link2files() :::::R:::: cp -f $_link $DESTDIR/$xfile"
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

# ubuntu dapper have very old manual_add_modules that don't work correctly
tcos_manual_add_modules()
{
	for mam_x in $(modprobe --set-version="${version}" --ignore-install \
	--show-depends "${1}" 2>/dev/null | awk '/^insmod/ { print $2 }'); do
		# Prune duplicates
		if [ -e "${DESTDIR}/${mam_x}" ]; then
			continue
		fi

		mkdir -p "${DESTDIR}/$(dirname "${mam_x}")"
		ln -s "${mam_x}" "${DESTDIR}/$(dirname "${mam_x}")"
		if [ -n "${verbose}" ] && [ "${verbose}" = "y" ]; then
			echo "Adding module ${mam_x}"
		fi
	done
}


force_add_module() {
        # force copy of module modprobe dont show as dependency
        this_module=$(modprobe --set-version="${version}" -l "${1}")
        if [ ! -e "${DESTDIR}/${this_module}" ]; then
                #echo "  WARNING, adding module $(basename ${this_module}), see Debian bug: #384043"
                mkdir -p "${DESTDIR}/$(dirname "${this_module}")"
                ln -s "${this_module}" "${DESTDIR}/$(dirname "${this_module}")"
        fi
        # end of force copy
}

function parse_pxelinuxcfg(){
        if [ $TCOS_NETBOOT_MENU ] && [ -z $2 ]; then
          TEMPLATE=${TCOS_CONF}/${TCOS_PXECFG_MENU}
          _verbose "(t-g-f) BOOT_MENU=1"
        else
          TEMPLATE=${TCOS_CONF}/${TCOS_PXECFG}
          _verbose "(t-g-f) BOOT_MENU=0"
        fi

        if [ $TCOS_NETBOOT_MENU_VESA ]; then
          TCOS_MENU_TYPE=vesamenu.c32
        else
          TCOS_MENU_TYPE=menu.c32
        fi

        if [ $TCOS_NETBOOT_HIDE_INSTALL ]; then
          TCOS_NETBOOT_HIDE_INSTALL_STRING=""
        else
          TCOS_NETBOOT_HIDE_INSTALL_STRING="#"
        fi

        if [ $TCOS_NETBOOT_HIDE_LOCAL ]; then
          TCOS_NETBOOT_HIDE_LOCAL_STRING=""
        else
          TCOS_NETBOOT_HIDE_LOCAL_STRING="#"
        fi

        if [ "$1" != "" ]; then
           DESTFILE=$1
        else
           DESTFILE=${TFTP_DIR}/pxelinux.cfg/default
        fi
        echo "## Generated file don't edit, edit $TEMPLATE instead"                  > ${DESTFILE}
        echo "## File generated by gentcos on $(date)"                               >> ${DESTFILE}
        cat ${TEMPLATE} >> ${DESTFILE}
        if [ -d /etc/tcos/conf.d/ ]; then
          if [ $TCOS_NETBOOT_MENU ] && [ -z $2 ]; then
             for i in /etc/tcos/templates/pxelinux.menu*.tpl; do
               [ -e $i ] && cat $i >> ${DESTFILE}
             done
          else
             for i in /etc/tcos/templates/pxelinux.nomenu*.tpl; do
               [ -e $i ] && cat $i >> ${DESTFILE}
             done
          fi
        fi
        sed -i -e "s/__TCOS_KERNEL__/${TCOS_KERNEL}/g"\
            -e "s/__DATE__/$(date)/g" \
            -e "s/__TCOS_MENU_TYPE__/${TCOS_MENU_TYPE}/g" \
            -e "s/__TCOS_NETBOOT_HIDE_INSTALL__/${TCOS_NETBOOT_HIDE_INSTALL_STRING}/g" \
            -e "s/__TCOS_NETBOOT_HIDE_LOCAL__/${TCOS_NETBOOT_HIDE_LOCAL_STRING}/g" \
            -e "s/__TCOS_MENU_PASSWD__/${TCOS_ROOT_PASSWD}/g" \
            -e "s/__TFTP_LOGO_JPG__/${TFTP_LOGO_JPG}/g" \
            -e "s/__TFTP_MSG_HELP1__/${TFTP_MSG_HELP1}/g" \
            -e "s/__TFTP_MSG_HELP2__/${TFTP_MSG_HELP2}/g" \
            -e "s/__TFTP_MSG__/${TFTP_MSG}/g" ${DESTFILE}
        _verbose "(t-g-f) MENU_FILE=$(cat ${DESTFILE})"

        if [ -d /etc/tcos/pxelinux.d ]; then
            for f in /etc/tcos/pxelinux.d/*.hook ; do
                exec $f || true
            done
        fi

}

