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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QMap>
#include <QString>

const QString KBaseUpdateUrl("UpdateUrl");
const QString KHostName("HostName");
const QString KPortNumber("PortNumber");
const QString KAdbDestPortNumber("AdbDestPortNumber");
const QString KForwardPortDest("ForwardPortDest");
const QString KFontPointSize("FontPointSize");
const QString KTargetType("TargetType");
const QString KTargetToolsPath("TargetToolsPath");
const QString KForwardMode("ForwardMode");

class Settings
{
public:
    Settings();
    void updateSetting(QString settingName, QString defaultValue);

    QString& operator[](const char* setting) {
        return mSettings[setting];
    }

    QString& operator[](const QString& setting) {
        return mSettings[setting];
    }

    void saveSettings();

    QMap<QString, QString>& getSettings() {return mSettings;}


  private:
    QMap<QString, QString> mSettings;
    QSettings mSettingsFile;
};

#endif // SETTINGS_H
