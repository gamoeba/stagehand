#ifndef STAGEHANDUPDATE_H
#define STAGEHANDUPDATE_H

#include <QObject>
#include <quazip/quazip.h>
#include "filedownloader.h"

class StagehandUpdate : public QObject
{
    Q_OBJECT

public:
    explicit StagehandUpdate(QString baseUpdates, QObject *parent = 0);
    void checkForUpdates(bool forceUpdate);
protected slots:
    void versionAvailable();
    void updateDownloaded();

private:
    void extractAll(QuaZip &archive, const QString& extractDir);
//    QString createApplyUpdateScript(const QString &extractDir, const QString &mainDir);

    FileDownloader* m_pDownloader;
    bool mForceUpdate;
    QString mBaseUpdates;
};

#endif // STAGEHANDUPDATE_H
