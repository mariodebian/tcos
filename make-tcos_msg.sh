#!/bin/sh
# Copyright (C) 2006,2007,2008  mariodebian at gmail
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

if [ "$1" = "help.msg" ]; then

_files=$(rgrep "DOCUMENTME" scripts/|grep -v svn | awk -F ":" '{print $1}')
#echo $_files
cat /dev/null > /tmp/make-tcos-$$
for _file in ${_files}; do
 echo $_file >> /tmp/make-tcos-$$
done

cat /tmp/make-tcos-$$ | sort | uniq > /tmp/make-tcos1-$$

echo -e '0008
Help about TCOS boot

Boot options:
'


for _file in $( cat /tmp/make-tcos1-$$); do
  #echo $_file
  num_of_vars=$(grep "DOCUMENTME" $_file | wc -l)
  for (( i=1; i<= $num_of_vars; i++)); do
   #echo $i
   _cmdvar=$(grep "DOCUMENTME" $_file | head -$i | tail -1 | awk '{print $3}')
   _cmdexp=$(grep "DOCUMENTME" $_file | head -$i | tail -1 | awk -F "|" '{print $2}' )
   _cmdvar_len=$(expr length ${_cmdvar})
   _tabs='\t'
   if [ ${_cmdvar_len} -lt 8 ]; then
     _tabs='\t\t'
   fi
   echo -e ${_cmdvar}${_tabs}${_cmdexp}'\n'
   _tabs='\t'
  done

  #sleep 2
done

rm -f /tmp/make-tcos-$$ /tmp/make-tcos1-$$

echo -e '
Press ^O08F1^O07 to return to main
'
else

echo -e 'logo.lss
07
Press 08F107 for help
Options   021:TCOS 64 Mb08 [default]  021:TCOS 32 Mb08
'
fi

