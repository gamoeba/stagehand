#!/bin/bash
cd $1
export PATH=$QT5_ROOT/bin:$PATH
qmake $STAGEHAND_ROOT CONFIG+=release
mingw32-make clean
mingw32-make -j3
cd -
