/*
 * Copyright (c) 2014 Gamoeba Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFileInfo fileInfo(argv[0]);
    w.setAppNameAndDirectory(fileInfo.baseName(), fileInfo.absoluteDir());

    if (argc>1) {
        QString hostName(argv[1]);
        w.setHostName(hostName);
    }
    if (argc ==3) {
        QString port(argv[2]);
        w.setPortNumber(port);
    }
    w.show();
    w.checkForUpdates();
    return a.exec();
}
