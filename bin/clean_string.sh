#!/bin/bash
#
#  Replace accents and other stuff
#  (break xmlrpc and xml.sax parser)
#

while IFS= read -t 1 -r INPUT
do
echo $INPUT | sed \
    -e 's/á/a/g' -e 's/Á/A/g' -e 's/à/a/g' -e 's/À/A/g' -e 's/ä/a/g' \
    -e 's/é/e/g' -e 's/É/E/g' -e 's/è/e/g' -e 's/È/E/g' -e 's/ë/e/g' \
    -e 's/í/i/g' -e 's/Í/I/g' -e 's/ì/i/g' -e 's/Ì/I/g' -e 's/ï/i/g' \
    -e 's/ó/o/g' -e 's/Ó/O/g' -e 's/ò/o/g' -e 's/Ò/O/g' -e 's/ö/o/g' \
    -e 's/ú/u/g' -e 's/Ú/U/g' -e 's/ù/u/g' -e 's/Ù/U/g' -e 's/ü/u/g'\
    -e 's/ñ/n/g' -e 's/Ñ/N/g' 
done


