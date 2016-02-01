#!/bin/bash
cd $1
export PATH=$QT5_ROOT/bin:$PATH
qmake $STAGEHAND_ROOT CONFIG+=release
make clean
make -j3
cd -
