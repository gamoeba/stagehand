#include <QString>
#include <QDebug>
#include <QBuffer>
#include "stagehandarchive.h"

#include <time.h>
#include "unzip.h"
#include "zip.h"


StagehandArchive::StagehandArchive()
{
}

char* extractFile(unzFile uf, ulong len) {
    char* buf = new char[len];
    unzOpenCurrentFile(uf);
    unzReadCurrentFile(uf, buf, len);
    unzCloseCurrentFile(uf);
    return buf;
}

void StagehandArchive::unzip(QString filename)
{
    char filename_inzip[256];
    unz_file_info file_info;

    unzFile uf = unzOpen(filename.toStdString().c_str());
    if (uf) {
        int err = unzGoToFirstFile(uf);
        while (err == UNZ_OK) {
            err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip)-1,NULL,0,NULL,0);
            ulong len = file_info.uncompressed_size;
            if ( strcmp("actors.txt", filename_inzip)==0 ) {
                char* buf = extractFile(uf, len);
                mSceneData = QString(QByteArray(buf, len));
                delete[] buf;
            } else if ( strcmp("screenshot.png", filename_inzip)==0 ) {
                char* buf = extractFile(uf, len);
                mScreenShot = QImage::fromData(QByteArray(buf,len),"PNG");
                delete[] buf;
            }
            err = unzGoToNextFile(uf);
        }
        unzClose(uf);
    }
}

void StagehandArchive::zip(QString filename)
{
    zip_fileinfo zi;

    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;

    time_t rawtime;
    time ( &rawtime );
    struct tm *timeinfo = localtime ( &rawtime );
    zi.tmz_date.tm_year = timeinfo->tm_year;
    zi.tmz_date.tm_mon = timeinfo->tm_mon;
    zi.tmz_date.tm_mday = timeinfo->tm_mday;
    zi.tmz_date.tm_hour = timeinfo->tm_hour;
    zi.tmz_date.tm_min = timeinfo->tm_min;
    zi.tmz_date.tm_sec = timeinfo->tm_sec;

    zipFile zf = zipOpen(filename.toStdString().c_str(), 0);
    zipOpenNewFileInZip(zf, "actors.txt", &zi,
                        NULL,0,NULL,0,NULL /* comment*/,
                        Z_DEFLATED, 9);
    zipWriteInFileInZip (zf, mSceneData.toStdString().c_str(), mSceneData.length());
    zipCloseFileInZip(zf);

    zipOpenNewFileInZip(zf, "screenshot.png", &zi,
                        NULL,0,NULL,0,NULL /* comment*/,
                        0, 0);
    QByteArray ba;
    QBuffer buffer( &ba );
    buffer.open( QBuffer::WriteOnly );
    mScreenShot.save( &buffer, "PNG" );
    buffer.close();
    zipWriteInFileInZip (zf, ba, ba.size());
    zipCloseFileInZip(zf);
    zipClose(zf, NULL);
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
