#include <QString>
#include <quazip/quazipfile.h>
#include <QDebug>
#include <QBuffer>
#include "stagehandarchive.h"

StagehandArchive::StagehandArchive()
{

}

void StagehandArchive::unzip(QString filename)
{
    QuaZip zip(filename);
    zip.open(QuaZip::mdUnzip);

    QuaZipFile file(&zip);

    for(bool f=zip.goToFirstFile(); f; f=zip.goToNextFile()) {
        QString fn = file.getActualFileName();
        QuaZipFileInfo64 info;
        file.getFileInfo(&info);
        file.open(QIODevice::ReadOnly);
        QByteArray byteArray = file.readAll();
        if (fn.compare("actors.txt")==0) {
            mSceneData = QString(byteArray);
        } else if (fn.compare("screenshot.png")==0) {
            mScreenShot = QImage::fromData(byteArray,"PNG");
        }
        //do something with the data
        file.close();
    }

    zip.close();
}

void StagehandArchive::zip(QString filename)
{
    QuaZip zip(filename);
    zip.open(QuaZip::mdCreate);

    QuaZipFile file(&zip);
    QuaZipNewInfo info("actors.txt");
    info.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther);
    file.open(QIODevice::WriteOnly,info);
    file.write(mSceneData.toStdString().c_str(), mSceneData.toStdString().length());
    file.close();

    QByteArray ba;
    QBuffer buffer( &ba );
    buffer.open( QBuffer::WriteOnly );
    mScreenShot.save( &buffer, "PNG" );
    buffer.close();
    QuaZipNewInfo info2("screenshot.png");
    info2.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther);
    file.open(QIODevice::WriteOnly,info2);
    file.write(ba);
    file.close();

    zip.close();
}

void StagehandArchive::setScreenShot(QImage img)
{
    mScreenShot = img;
}

void StagehandArchive::setSceneData(QString data)
{
    mSceneData = data;
}

QImage StagehandArchive::getScreenShot()
{
    return mScreenShot;
}

const QString& StagehandArchive::getSceneData()
{
    return mSceneData;
}
