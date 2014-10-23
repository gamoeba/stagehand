#include <QMutexLocker>
#include "recordthread.h"
#include "mainwindow.h"

RecordThread::RecordThread(QObject *parent) :
    QThread(parent)
{
}

void RecordThread::startRecord()
{
    mRecordedFrames.clear();
    mFetchFrameIndex = 0;
    mRunning = true;
    this->start();
}

void RecordThread::stopRecord()
{
    mRunning = false;
}

bool RecordThread::hasMoreFrames()
{
    QMutexLocker lock(&mFrameLock);
    return (mFetchFrameIndex < mRecordedFrames.size());
}

std::string RecordThread::getFrame()
{
    return mRecordedFrames[mFetchFrameIndex++];
}

void RecordThread::run()
{
    mClient = new SocketClient;
    mClient->sendCommand(MainWindow::settings.mHostName, MainWindow::settings.mPortNumber.toUInt(), "start_record\n");

    int frameCount = 0;
    while (mRunning)
    {
        std::string resp = mClient->readSizedString();
        if (resp.length()>0)
        {
            QMutexLocker lock(&mFrameLock);
            mRecordedFrames.push_back(resp);
            emit frameAvailable();
        } else {
            qDebug() << "zero sized frame";
        }
    }

    mClient->sendCommand(MainWindow::settings.mHostName, MainWindow::settings.mPortNumber.toUInt(), "stop_record\n");

    delete mClient;
    mClient = NULL;
}
