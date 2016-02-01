#!/bin/bash
BASEDIR=output/stagehand-qtdeps_${VERSION}_${ARCH}
mkdir -p $BASEDIR/DEBIAN
cp deb/qtdeps_control $BASEDIR/DEBIAN/control
echo "Architecture: "$ARCH >> $BASEDIR/DEBIAN/control
echo "Version: "$VERSION >> $BASEDIR/DEBIAN/control

mkdir -p $BASEDIR/usr/share/stagehand/lib
mkdir -p $BASEDIR/usr/share/stagehand/plugins/platforms
mkdir -p $BASEDIR/usr/share/stagehand/plugins/xcbglintegrations

while read -r line
do
  cp $QT5_ROOT/$line $BASEDIR/usr/share/stagehand/$line
done < qtdeps 

echo "Installed-Size: "`du -s -k $BASEDIR | awk '{ print $1 }'` >> $BASEDIR/DEBIAN/control

fakeroot dpkg-deb --build $BASEDIR
