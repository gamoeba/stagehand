#include <QString>
#include <QFile>
#include <QApplication>
#include <QUrl>
#include <QMessageBox>
#include <QProcess>
#include <QNetworkProxyFactory>
#include <QBuffer>

#include "stagehandupdate.h"
#include "version.h"
#include "utils.h"
#include "mainwindow.h"
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

StagehandUpdate::StagehandUpdate(QString baseUpdates, QObject* parent)
    :QObject(parent), mBaseUpdates(baseUpdates)
{
}

// Automatic update Functions

void StagehandUpdate::checkForUpdates(bool forceUpdate)
{
    QNetworkProxyFactory::setUseSystemConfiguration(true);

    mForceUpdate = forceUpdate;
    QString versionFile;
    versionFile.sprintf("/version_info_%s.txt",versionSuffix);

    QUrl versionUrl(mBaseUpdates + versionFile);
    m_pDownloader = new FileDownloader(versionUrl, this);

    QObject::connect(m_pDownloader, SIGNAL(downloaded()), SLOT(versionAvailable()));
}

void StagehandUpdate::versionAvailable()
{
    QMessageBox msgBox;
    QByteArray data = m_pDownloader->downloadedData();
    QString versionInfo(data);

    QTextStream stream(data);
    double versionNumber = stream.readLine().toDouble();
    if (mForceUpdate || versionNumber > STAGEHAND_VERSION)
    {

        msgBox.setText("Update Available");
        msgBox.setInformativeText(versionInfo);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.exec();
        int res = msgBox.result();

        if (res == QMessageBox::Yes)
        {
            QString updateFile;
            updateFile.sprintf("/%s_update%g.zip",versionSuffix, versionNumber);

            QUrl versionUrl(mBaseUpdates + updateFile);
            m_pDownloader = new FileDownloader(versionUrl, this);

            QObject::connect(m_pDownloader, SIGNAL(downloaded()), SLOT(updateDownloaded()));

        }
    }
}

void StagehandUpdate::updateDownloaded()
{
    const QString appName = "stagehand";
    QByteArray data = m_pDownloader->downloadedData();

    QString mainPath = appendPath(QDir::homePath(),".stagehand");
    QString extractDir = appendPath(mainPath, "update");
    QString startPath = appendPath(QApplication::applicationDirPath(), "../" +appName);

    QBuffer buffer( &data );
    buffer.open( QBuffer::ReadOnly );

    QuaZip zipFile(&buffer);
    extractAll(zipFile, extractDir);
    buffer.close();
    if (true) //exitCode == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Restart application to complete update?");
        msgBox.setInformativeText("Updates will only be applied after restarting");
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.exec();
        int res = msgBox.result();

        if (res == QMessageBox::Yes)
        {
            // solve the chicken and egg problem
			QProcess::startDetached(startPath);
            QApplication::quit();
        }
    }
}

void StagehandUpdate::extractAll(QuaZip& archive, const QString &extractDir )
{
    // extracts all files from the archive and saves
    // them onto disk
    //
    // just a simple implementation for evaluation.
    // error handling has not been implemented yet!
    archive.open(QuaZip::mdUnzip);
    QuaZipFile file(&archive);

    QDir dir(extractDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    for( bool f = archive.goToFirstFile(); f; f = archive.goToNextFile() )
    {
        // set source file in archive
        QString filePath = archive.getCurrentFileName();
        QString newPath = appendPath(extractDir, filePath);

        if (!filePath.endsWith("/"))
        {
            QuaZipFileInfo64 info;
            file.getFileInfo(&info);
            QFile::Permissions perm = info.getPermissions();
            file.open( QIODevice::ReadOnly);

            // create a bytes array and write the file data into it
            QByteArray ba = file.readAll();
            // close the source file
            file.close();
            // set destination file
            qDebug() << "writing" << newPath;
            QFile dstFile( newPath );
            // open the destination file
            dstFile.open( QIODevice::WriteOnly | QIODevice::Truncate);
            // write the data from the bytes array into the destination file
            dstFile.write( ba );
            //close the destination file
            dstFile.setPermissions(perm);
            dstFile.close();
        }
        else
        {
            QDir dir(newPath);
            if (!dir.exists()) {
                dir.mkpath(".");
            }
        }
    }
    archive.close();
}



