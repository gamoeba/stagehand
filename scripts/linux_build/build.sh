#!/bin/bash
if [ -z "$QT5_64ROOT" ]
then
  echo "Must set QT5_64ROOT"
  exit 1
fi
mkdir -p linux64
export STAGEHAND_ROOT=../../..

echo "------ 64 bit build ------ "
export QT5_ROOT=$QT5_64ROOT
. build_release.sh linux64
echo "------ 32 bit build ------ "
if [ -z "$QT5_32ROOT" ]
then
  echo "Must set QT5_32ROOT in order to perform 32 bit build"
  exit 1
fi
mkdir -p linux32
export QT5_ROOT=$QT5_32ROOT
. build_release.sh linux32

