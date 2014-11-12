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

#include "settings.h"

const QString KBaseUpdateUrl("UpdateUrl");
const QString KNodeName("NodeName");
const QString KNodeId("NodeId");
const QString KNodeVisible("NodeVisible");
const QString KNodeChildrenName("NodeChildrenName");
const QString KNodePropertiesName("NodePropertiesName");
const QString KCameraNodeName("CameraNode");
const QString KCameraNodeProjectionMatrixName("CameraNodeProjectionMatrixName");
const QString KCameraNodeViewMatrixName("CameraNodeViewMatrixName");
const QString KCameraNodeAspectRatioName("CameraNodeAspectRatioName");
const QString KNodeWorldMatrixName("NodeWorldMatrixName");
const QString KNodeSizeName("NodeSizeName");
const QString KHostName("HostName");
const QString KPortNumber("PortNumber");
const QString KAdbDestPortNumber("AdbDestPortNumber");
const QString KCmdGetScene("GetScene");
const QString KCmdSetProperties("SetProperties");
const QString KFontPointSize("FontPointSize");

Settings::Settings()
{
}

void Settings::loadSettings(QString fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);
    mNodeName = updateSetting(settings, KNodeName, "Name");
    mNodeID = updateSetting(settings, KNodeId, "id");
    mNodeVisible = updateSetting(settings, KNodeVisible, "visible");
    mNodePropertiesName = updateSetting(settings, KNodePropertiesName, "properties");
    mNodeChildrenName = updateSetting(settings, KNodeChildrenName, "children");
    mCameraNodeName = updateSetting(settings, KCameraNodeName, "camera");
    mPropProjectionMatrixName = updateSetting(settings, KCameraNodeProjectionMatrixName, "projMatrix");
    mPropViewMatrixName = updateSetting(settings, KCameraNodeViewMatrixName, "viewMatrix");
    mPropAspectRatioName = updateSetting(settings, KCameraNodeAspectRatioName, "aspectRatio");
    mPropNodeWorldMatrixName = updateSetting(settings, KNodeWorldMatrixName, "worldMatrix");
    mPropNodeSizeName = updateSetting(settings, KNodeSizeName, "size");
    mHostName = updateSetting(settings, KHostName, "127.0.0.1");
    mPortNumber = updateSetting(settings, KPortNumber, "5001");
    mAdbForwardPort = updateSetting(settings, KAdbDestPortNumber, "");
    mCmdGetScene = updateSetting(settings, KCmdGetScene, "get_scene");
    mCmdSetProperties = updateSetting(settings, KCmdSetProperties, "set_properties");
    mFontPointSize = updateSetting(settings, KFontPointSize, "10");
    mBaseUpdateUrl = updateSetting(settings, KBaseUpdateUrl, "http://www.gamoeba.com/stagehand_updates");
    settings.sync();
}

QString Settings::updateSetting(QSettings& settings, QString settingName, QString defaultValue ) {
    QString value = settings.value(settingName,"").toString();
    if (value.length()==0) {
        value = defaultValue;
        settings.setValue(settingName, value);
    }
    return value;
}


