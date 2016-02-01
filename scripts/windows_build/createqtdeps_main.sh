#!/bin/bash
export QT5_ROOT=$QT5_32ROOT
mkdir -p output/bin
mkdir -p output/plugins/platforms

cp win32/release/stagehand.exe output/bin
cp -R ../platform_scripts/stagehand output
cp -R ../platform_scripts/android output/bin/
cp -R ../platform_scripts/tizen output/bin/
cp -R ../platform_scripts/ubuntu output/bin/

while read -r line
do
  cp $QT5_ROOT/$line output/$line
done < qtdeps 

