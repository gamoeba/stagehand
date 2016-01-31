#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T17:43:30
#
#-------------------------------------------------

QT       += core gui opengl network widgets

TARGET = stagehand
TEMPLATE = app

ICON = resources/stagehand.icns

VPATH += ./src

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
    version.cpp \
    utils.cpp \
    settingsdialog.cpp \
    performancedialog.cpp \
    circularbuffer.cpp \
    initialsettingsdialog.cpp \
    glprogram.cpp

VPATH += extsrc

SOURCES += ioapi.c mztools.c unzip.c zip.c
HEADERS += unzip.h zip.h
win32 {
    SOURCES += iowin32.c
}

unused {
    SOURCES += miniunz.c minizip.c
}

INCLUDEPATH += inc
INCLUDEPATH += extsrc
INCLUDEPATH += $$PWD/extlibs/include/

VPATH += inc
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
    stagehandarchive.h \
    stagehandupdate.h \
    utils.h \
    settingsdialog.h \
    consts.h \
    performancedialog.h \
    circularbuffer.h \
    initialsettingsdialog.h \
    glprogram.h

VPATH += forms
FORMS    += mainwindow.ui \
    settingsdialog.ui \
    performancedialog.ui \
    initialsettingsdialog.ui


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
    android/AndroidManifest.xml \
    shaderLines.fsh \
    shaderLines.vsh

RESOURCES += \
    resources/stagehand.qrc

unix:!macx {
LIBS+= -L$$PWD/extlibs/Linux/$$QMAKE_HOST.arch
}
macx {
LIBS+= -L$$PWD/extlibs/osx
}

LIBS+= -lz

QMAKE_CXXFLAGS += -isystem extsrc
DEFINES += NOCRYPT


