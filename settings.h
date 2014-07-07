#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
public:
    Settings();
    void loadSettings();
    QString updateSetting(QSettings &settings, QString settingName, QString defaultValue);

    QString m_sSettingsFile;

    QString mNodeName;
    QString mNodeID;
    QString mNodeVisible;
    QString mNodeChildrenName;
    QString mNodePropertiesName;

    QString mCameraNodeName;
    QString mPropProjectionMatrixName;
    QString mPropViewMatrixName;
    QString mPropAspectRatioName;

    QString mPropNodeWorldMatrixName;
    QString mPropNodeSizeName;
    QString mHostName;
    QString mPortNumber;
};

#endif // SETTINGS_H
