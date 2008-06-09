#!/bin/bash

ALL_VARS1=$(rgrep "\$TCOS_" * |grep -v svn |grep -v "\.py" |grep -v "debian/" | awk -F'TCOS_' '{print "TCOS_"$2}' | \
 awk '{print $1}' | awk -F"=" '{print $1}' | awk -F"/" '{print $1}' | \
 sed 's/)//g' | sed 's/}//g' | sed 's/"//g'| sed 's/;//g' |sort | uniq)

ALL_VARS2=$(rgrep "^TCOS_" conf/ |grep -v svn | awk -F":" '{print $2}' |awk -F"=" '{print $1}' |sort | uniq)

ALL_VARS=$(echo $ALL_VARS1 $ALL_VARS2 | sort | uniq)

eline(){
  echo "------------------------------------------------------------------------------------------------------------------"
}

TEMPLATES="conf/tcos.conf.all conf/tcos.conf.low conf/tcos.conf.nfs"

is_in_template() {
  value=""
  for tpl in $TEMPLATES; do
    if [ $(grep -c "^${1}=" $tpl) != 0 ]; then
      tmp=$(grep "^${1}=" $tpl | awk -F"=" '{print $2}')
      value="$value $(basename $tpl):'$tmp'"
    fi
  done
  echo $value
}


printf "%30s |%5s| %8s| %10s| %5s| %36s|\n" "VARNAME" "base" "value" "" "needed in base" "in templates"
eline


number=0
for VAR in $ALL_VARS; do
  base=$(grep -c "^${VAR}=" conf/base.conf)
  basevalue=$( grep "^${VAR}=" conf/base.conf | awk -F"=" '{print $2}')
  tcos=$(grep -c "^${VAR}=" conf/tcos.conf)
  iit=$(is_in_template $VAR)
  if [ $base = 0 ] && [ $tcos = 0 ]; then
    needbase="FIXME"
  else
    needbase=""
  fi
  printf "%30s |%5s| %8s| %15s| %5s| %40s|\n" "$VAR" "$base" "$basevalue" "" "$needbase" "$iit"
  tnumber=$((number/5))
  [ $number = $((tnumber*5)) ] && eline
  number=$((number+1))
done
