/****************************************************************************
Copyright (C) 2014 Gamoeba Ltd
All rights reserved

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

****************************************************************************/

#include "settings.h"

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


