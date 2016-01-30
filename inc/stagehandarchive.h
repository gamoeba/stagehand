#ifndef STAGEHANDARCHIVE_H
#define STAGEHANDARCHIVE_H

#include <QString>
#include <QImage>

class StagehandArchive
{
public:
    StagehandArchive();
    void unzip(QString filename);
    void zip(QString filename);

    void setScreenShot(QImage img);
    void setSceneData(QString data);
    QImage getScreenShot();
    const QString &getSceneData();
private:
    QImage mScreenShot;
    QString mSceneData;
};

#endif // STAGEHANDARCHIVE_H
