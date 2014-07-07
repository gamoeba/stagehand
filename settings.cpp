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

Settings::Settings()
{
    m_sSettingsFile = "stagehand.ini";
}

void Settings::loadSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
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


