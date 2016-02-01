#!/bin/bash
echo "------ 32 bit build ------ "
if [ -z "$QT5_32ROOT" ]
then
  echo "Must set QT5_32ROOT in order to perform 32 bit build"
  exit 1
fi
mkdir -p win32
export STAGEHAND_ROOT=../../..
export QT5_ROOT=$QT5_32ROOT
. build_release.sh win32

