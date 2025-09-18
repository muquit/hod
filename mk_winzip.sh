#!/bin/bash
# make the zip archive for windows
# Sep-17-2025 
v=$(cat VERSION)
cp="/bin/cp -fv"
dir=hod-${v}-win_amd64
zip_file=${dir}.zip
if [[ -d ${dir} ]]; then
    /bin/rm -f ${dir}/*
fi
mkdir -p ${dir}
${cp} ./hod-${v}.exe ${dir}/hod.exe
${cp} README.md ${dir}
${cp} ChangeLog.md ${dir}
zip -r ${zip_file} ${dir}
unzip -l ${zip_file}
${cp} ${zip_file} ./bin/
git rm -f bin/md5.txt
(cd bin; md5sum * > md5.txt)
