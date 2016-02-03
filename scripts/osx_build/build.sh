#!/bin/bash
echo "------ OSX build (only 64 Bit) ------ "
if [ -z "$QT5_ROOT" ]
then
  echo "Must set QT5_ROOT in order to perform build"
  exit 1
fi
mkdir -p osx
export STAGEHAND_ROOT=../../..
. build_release.sh osx 

