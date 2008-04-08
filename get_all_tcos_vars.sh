#!/bin/bash

ALL_VARS=$(rgrep "\$TCOS_" * |grep -v svn | awk -F'TCOS_' '{print "TCOS_"$2}' | \
 awk '{print $1}' | awk -F"=" '{print $1}' | awk -F"/" '{print $1}' | \
 sed 's/)//g' | sed 's/}//g' | sed 's/"//g'| sed 's/;//g' |sort | uniq)


TEMPLATES="conf/tcos.conf.all conf/tcos.conf.low"

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


printf "%30s %5s %8s\n" "VARNAME" "base" "value"
echo "-------------------------------------------------"


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
  [ $number = $((tnumber*5)) ] && echo "-------------------------------------------------------------------------------------------------------------------"
  number=$((number+1))
done
