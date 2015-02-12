#ifndef PERFORMANCEDIALOG_H
#define PERFORMANCEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QVector>
#include "messagereceived.h"
#include "socketclient.h"

namespace Ui {
class PerformanceDialog;
}

class PerformanceDialog : public QDialog, public IMessageReceived
{
    Q_OBJECT

public:
    explicit PerformanceDialog(QWidget *parent = 0);
    ~PerformanceDialog();

    void setConnection(QString hostAddress, int portNumber);

private:
    Ui::PerformanceDialog *ui;
    SocketClient mClient;
    QGraphicsScene* mScene;
    bool mFirstVSync;
    // IMessageReceived interface
public:
    void MessageReceived(QString &recv);

public slots:
    void pause(bool paused);
private:
    void addUpdate();
    void addRender();
    void addSwapBuffers();
    void addVSync(double time);

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
};

#endif // PERFORMANCEDIALOG_H
