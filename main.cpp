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
#include <version.h>
#include "stagehandupdate.h"

int main(int argc, char *argv[])
{
    bool forceUpdate = false;

    if (argc==2 && strcmp(argv[1],"--force-update")==0)
    {
        forceUpdate = true;
    }

    if (argc==2 && strcmp(argv[1],"--version")==0)
    {
        printf("%g\n", STAGEHAND_VERSION);
        exit(0);
    }

    if (argc==2 && strcmp(argv[1],"--versioninfo")==0)
    {
        QFile versioninfo(":/stagehand/versioninfo.txt");
        versioninfo.open(QIODevice::ReadOnly);
        QByteArray ver = versioninfo.readAll();
        QString str(ver);
        printf("%g\n\n%s\n", STAGEHAND_VERSION, str.toStdString().c_str());
        exit(0);
    }

    QApplication a(argc, argv);
    QFileInfo fileInfo(argv[0]);
    MainWindow w;


    if (argc>1) {

        QString hostName(argv[1]);
        w.setHostName(hostName);
    }
    if (argc ==3) {
        QString port(argv[2]);
        w.setPortNumber(port);
    }

    w.show();
    StagehandUpdate updater(w.settings[KBaseUpdateUrl]);
    updater.checkForUpdates(forceUpdate);
    return a.exec();
}
