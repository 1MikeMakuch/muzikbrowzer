#!/bin/bash

zfile=$*

cd /cygdrive/c/mkm/src
rm $zfile.zip
find ./muzik -type f | egrep -v \
"md5|CVS|mbpwDlg|mydiff|Inno|PreBuild|Publish|scripts|mbpw|Debug\
|Release|\.exe|.ncb$|.opt$|.plg$"\
| zip $zfile -@
