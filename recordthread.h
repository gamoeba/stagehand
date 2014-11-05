#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H

#include <QThread>
#include <QMutex>
#include "socketclient.h"
#include <vector>

class RecordThread : public QThread
{
    Q_OBJECT
public:
    explicit RecordThread(QObject *parent = 0);

    void startRecord();
    void stopRecord();
    bool hasMoreFrames();
    std::string getFrame();
signals:
    void frameAvailable();
    void recordingFinished();

public slots:


protected:
    void run();


private:
    SocketClient* mClient;
    bool mRunning;
    std::vector<std::string> mRecordedFrames;
    unsigned int mFetchFrameIndex;
    QMutex mFrameLock;
};

#endif // RECEIVEUPDATETHREAD_H
