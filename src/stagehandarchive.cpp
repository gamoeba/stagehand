#include <QString>
#include <quazip/quazipfile.h>
#include <QDebug>
#include <QBuffer>
#include "stagehandarchive.h"

#include "unzip.h"

StagehandArchive::StagehandArchive()
{

}

void StagehandArchive::unzip(QString filename)
{
    char filename_inzip[256];
    unz_file_info64 file_info;

    unzFile uf = unzOpen((voidpf)filename.toStdString().c_str());
    if (uf) {
        int err = unzGoToFirstFile(uf);
        while (err == UNZ_OK) {
            err = unzGetCurrentFileInfo64(uf,&file_info,filename_inzip,sizeof(filename_inzip)-1,NULL,0,NULL,0);
            if ( strcmp("actors.txt", filename_inzip)==0 ) {
                ulong len = file_info.uncompressed_size;
                char* buf = new char[len];
                unzOpenCurrentFile(uf);
                unzReadCurrentFile(uf, buf, len);
                mSceneData = QString(QByteArray(buf, len));
                delete[] buf;
                unzCloseCurrentFile(uf);
            }
            err = unzGoToNextFile(uf);
        }
        unzClose(uf);
    }
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
