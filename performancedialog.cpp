#include "performancedialog.h"
#include "ui_performancedialog.h"
#include "socketclient.h"
#include "consts.h"

#include <QGraphicsRectItem>

const double KLineSize = 10;
const double KLineDrawSize = 5;
const double KScaleX = 10000.0;

PerformanceDialog::PerformanceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerformanceDialog),
    mFirstVSync(true),
    mCurrentRow(0)
{
    ui->setupUi(this);
    mScene = new QGraphicsScene;//(QRectF(-1,-1,0,10));
    //mScene->setSceneRect(QRectF(0,0,100,100));

//    int MouseCount =7;
//    for (int i = 0; i < MouseCount; ++i) {
//        Mouse *mouse = new Mouse;
//        mouse->setPos(::sin((i * 6.28) / MouseCount) * 200,
//                      ::cos((i * 6.28) / MouseCount) * 200);
//        mScene->addItem(mouse);
//    }

    //ui->graphicsView->fitInView(QRectF(0,0,32,1000));
    ui->graphicsView->setScene(mScene);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    //mScene->setSceneRect(QRectF(-1,-1,0,10));
    //ui->graphicsView->setSceneRect(

}

PerformanceDialog::~PerformanceDialog()
{
    delete mScene;
    delete ui;
}

void PerformanceDialog::setConnection(QString hostAddress, int portNumber)
{
    mFirstVSync = true;
    mClient.connectSocket(hostAddress, portNumber);
    mClient.sendCommand(KDaliCmdPerformanceMarkersOn);
    mClient.waitForMessages(this);
}

void PerformanceDialog::MessageReceived(QString& recv)
{
    //qWarning() << recv;
    QStringList args = recv.split(QRegExp("( |\n)"));
    if (args.length()>=3) {
        double time = args[0].toDouble();
        if (args[2].compare("V_SYNC")==0) {
            if (!mFirstVSync) {
                double vsyncduration = (time - mLastVSync);
                mLastVSync = time;
                if (vsyncduration < 0.032) {
                    addVSync(vsyncduration);
                    mCurrentRow++;
                } else {
                    mCurrentRow+=5;
                }
            }
            mFirstVSync = false;
        } else if (!mFirstVSync) {
            // only process other messages if we have already got a vsync, since the timings are impossible without it
            if (args[2].compare("UPDATE_START")==0) {
                mUpdateStart = time;
                mUpdateVSync = mLastVSync;
                mUpdateRow = mCurrentRow;
            } else if (args[2].compare("UPDATE_END")==0) {
                mUpdateEnd = time;
                addUpdate();
            } else if (args[2].compare("RENDER_START")==0) {
                mRenderStart = time;
                mRenderVSync = mLastVSync;
                mRenderRow = mCurrentRow;
            } else if (args[2].compare("RENDER_END")==0) {
                mRenderEnd = time;
                addRender();
            } else if (args[2].compare("SWAP_START")==0) {
                mSwapStart = time;
                mSwapVSync = mLastVSync;
                mSwapRow = mCurrentRow;
            } else if (args[2].compare("SWAP_END")==0) {
                mSwapEnd = time;
                addSwapBuffers();
            }
        }
    }

}

void PerformanceDialog::addUpdate()
{
    mScene->addRect(QRectF((mUpdateStart-mUpdateVSync)*KScaleX,mUpdateRow*KLineSize,(mUpdateEnd-mUpdateStart)*KScaleX, KLineDrawSize),QPen(Qt::green),QBrush(Qt::green,Qt::SolidPattern));
}

void PerformanceDialog::addRender()
{
    mScene->addRect(QRectF((mRenderStart-mRenderVSync)*KScaleX,mRenderRow*KLineSize,(mRenderEnd-mRenderStart)*KScaleX, KLineDrawSize),QPen(Qt::blue),QBrush(Qt::blue,Qt::SolidPattern));
}

void PerformanceDialog::addSwapBuffers()
{
    mScene->addRect(QRectF((mSwapStart-mSwapVSync)*KScaleX, mSwapRow*KLineSize, (mSwapEnd-mSwapStart)*KScaleX, KLineDrawSize), QPen(Qt::yellow), QBrush(Qt::yellow,Qt::SolidPattern));
}

void PerformanceDialog::addVSync(double time)
{
    double xpos = time * KScaleX;
    QGraphicsLineItem* item = mScene->addLine(QLineF(xpos, mCurrentRow*KLineSize, xpos, mCurrentRow*KLineSize+KLineSize), QPen(Qt::red));
    item->ensureVisible();
    ui->graphicsView->fitInView(0,KLineSize*mCurrentRow-ui->graphicsView->height(),320,ui->graphicsView->height());

}

void PerformanceDialog::pause(bool paused)
{
   if (paused) {
       mClient.sendCommand(KDaliCmdPerformanceMarkersOff);
   } else {
       mFirstVSync = true;
       mClient.sendCommand(KDaliCmdPerformanceMarkersOn);
   }
}
