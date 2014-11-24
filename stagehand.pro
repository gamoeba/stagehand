#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T17:43:30
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stagehand
TEMPLATE = app

ICON = stagehand.icns

SOURCES += main.cpp\
        mainwindow.cpp \
    treemodel.cpp \
    jsonitem.cpp \
    tablemodel.cpp \
    dataobject.cpp \
    glwidget.cpp \
    nodeobject.cpp \
    sceneobject.cpp \
    settings.cpp \
    socketclient.cpp \
    delegate.cpp \
    filedownloader.cpp \
    stagehandarchive.cpp \
    version.cpp

INCLUDEPATH += /usr/local/include/quazip/

HEADERS  += mainwindow.h \
    treemodel.h \
    jsonitem.h \
    tablemodel.h \
    dataobject.h \
    glwidget.h \
    nodeobject.h \
    sceneobject.h \
    settings.h \
    socketclient.h \
    delegate.h \
    filedownloader.h \
    version.h \
    stagehandarchive.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    folder_invisible.png \
    folder_visible.png \
    folder.png \
    invisible.png \
    leaf_invisible.png \
    leaf_visible.png \
    visible.png \
    shader.fsh \
    shader.vsh \
    android/AndroidManifest.xml

RESOURCES += \
    stagehand.qrc

LIBS+=  -lquazip

macx {
LIBS+= -L$$PWD/extlibs/osx
}

android {
LIBS+= -L$$PWD/extlibs/Android
}

ios {
LIBS+= -L$$PWD/extlibs/ios
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android



