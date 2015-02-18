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
#include <QDir>
#include "settings.h"
#include <QDebug>



Settings::Settings()
    :mSettingsFile(QDir::homePath()+QDir::separator()+".stagehand"+QDir::separator()+"stagehand.ini",
                   QSettings::IniFormat)
{
    mSettings[KHostName] ="127.0.0.1";
    mSettings[KPortNumber] = "3031";
    mSettings[KForwardPortDest] = "tcp:3031";
    mSettings[KFontPointSize] = "10";
    mSettings[KBaseUpdateUrl] = "http://www.gamoeba.com/stagehand_updates";
    mSettings[KTargetType] = "android";
    mSettings[KTargetToolsPath] = "~/bin";
    mSettings[KForwardMode] = "forward";

    QMap<QString, QString>::iterator iter;
    for (iter = mSettings.begin();iter != mSettings.end();iter++) {
        updateSetting(iter.key(), iter.value());
    }

}

void Settings::updateSetting(QString settingName, QString defaultValue ) {
    QString value = mSettingsFile.value(settingName,"").toString();
    if (value.length()==0) {
        qDebug() << "reset default"<< settingName;
        value = defaultValue;
        mSettingsFile.setValue(settingName, value);
    } else {
        mSettings[settingName] = value;
    }
}

void Settings::saveSettings()
{
    QMap<QString, QString>::iterator iter;
    for (iter = mSettings.begin();iter !=mSettings.end();++iter)
    {
        mSettingsFile.setValue(iter.key(),iter.value());
    }
    mSettingsFile.sync();
}


