#ifndef PERFORMANCEDIALOG_H
#define PERFORMANCEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QVector>
#include "messagereceived.h"
#include "socketclient.h"

class CircularBuffer;

namespace Ui {
class PerformanceDialog;
}

class PerformanceDialog : public QDialog, public IMessageReceived
{
    Q_OBJECT

public:
    explicit PerformanceDialog(QWidget *parent = 0);
    ~PerformanceDialog();

    void setConnection(QString hostAddress, quint16 portNumber);

protected:
    void resizeEvent(QResizeEvent* event);
//    bool eventFilter(QObject * obj, QEvent * event);

private:
    Ui::PerformanceDialog *ui;
    SocketClient mClient;
    QGraphicsScene* mScene;
    bool mFirstVSync;
    // IMessageReceived interface
public:
    void MessageReceived(QString &recv);
signals:
    void messageAvailable();

public slots:
    void pause();
    void start();
    void processMessages();
private:
    void addUpdate();
    void addRender();
    void addSwapBuffers();
    void addVSync(double time);

    QStringList mMessages;
    QMutex mMessagesMutex;
    QString mHostAddress;
    quint16 mPortNumber;

    double mUpdateVSync;
    double mUpdateStart;
    double mUpdateEnd;
    int mUpdateRow;
    double mRenderStart;
    double mRenderEnd;
    double mRenderVSync;
    int mRenderRow;
    double mSwapStart;
    double mSwapEnd;
    double mSwapVSync;
    int mSwapRow;
    double mLastVSync;
    int mCurrentRow;

    CircularBuffer* mUpdateValues;
    CircularBuffer* mRenderValues;
    CircularBuffer* mSwapValues;
    void addBar(double startTime, double duration, double row, QColor startCol, QColor midCol, QColor endCol);
};

#endif // PERFORMANCEDIALOG_H
