#!/bin/bash
STAGEHAND_BIN=$1
VERSION=`$STAGEHAND_BIN/stagehand --version`

BASEDIR=output/stagehand_${VERSION}_${ARCHNAME}
mkdir -p $BASEDIR/DEBIAN
cp deb/control $BASEDIR/DEBIAN
cp deb/postrm $BASEDIR/DEBIAN
cp deb/postinst $BASEDIR/DEBIAN
echo "Architecture: "$ARCHNAME >> $BASEDIR/DEBIAN/control
echo "Version: "$VERSION >> $BASEDIR/DEBIAN/control

mkdir -p $BASEDIR/usr/share/applications
mkdir -p $BASEDIR/usr/share/doc/stagehand
mkdir -p $BASEDIR/usr/share/stagehand
mkdir -p $BASEDIR/usr/share/stagehand/bin
mkdir -p $BASEDIR/usr/share/stagehand/lib
mkdir -p $BASEDIR/usr/share/stagehand/bin/android
mkdir -p $BASEDIR/usr/share/stagehand/bin/tizen
mkdir -p $BASEDIR/usr/share/stagehand/bin/ubuntu

cp deb/Stagehand.desktop $BASEDIR/usr/share/applications/
cp deb/copyright $BASEDIR/usr/share/doc/stagehand/
cp ../../resources/stagehand.png $BASEDIR/usr/share/stagehand/stagehand.png
cp ../platform_scripts/stagehand $BASEDIR/usr/share/stagehand/stagehand
cp $STAGEHAND_BIN/stagehand $BASEDIR/usr/share/stagehand/bin/
cp -R ../platform_scripts/android $BASEDIR/usr/share/stagehand/bin
cp -R ../platform_scripts/tizen $BASEDIR/usr/share/stagehand/bin
cp -R ../platform_scripts/ubuntu $BASEDIR/usr/share/stagehand/bin

chmod -R 755 $BASEDIR
chmod 644 $BASEDIR/usr/share/applications/Stagehand.desktop
chmod 644 $BASEDIR/usr/share/stagehand/stagehand.png

echo "Installed-Size: "`du -s -k $BASEDIR | awk '{ print $1 }'` >> $BASEDIR/DEBIAN/control

fakeroot dpkg-deb --build $BASEDIR

zip -r --symlinks "output/${PLATFORM}_${ARCHNAME}_update$VERSION.zip" $BASEDIR.deb
$STAGEHAND_BIN/stagehand --versioninfo > output/version_info_${PLATFORM}_${ARCHNAME}.txt
