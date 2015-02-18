#include "performancedialog.h"
#include "ui_performancedialog.h"
#include "socketclient.h"
#include "consts.h"
#include "circularbuffer.h"

#include <QGraphicsRectItem>
#include <QWheelEvent>

const double KLineSize = 10;
const double KLineDrawSize = 5;
const double KScaleX = 1000.0;

class GridScene : public QGraphicsScene
{
public:
    GridScene() {}
    GridScene(qreal x, qreal y, qreal w, qreal h)
        : QGraphicsScene(x, y, w, h)
    { }

protected:
    void drawBackground(QPainter *painter, const QRectF &rect)
    {
        QGraphicsScene::drawBackground(painter, rect);
        const int gridSizeX = 1.0f;
        const int gridSizeY = 60.0f*KLineSize;

        qreal left = int(rect.left()) - (int(rect.left()) % gridSizeX);
        qreal top = int(rect.top()) - (int(rect.top()) % gridSizeY);

        QVarLengthArray<QLineF, 100> lines;

        for (qreal x = left; x < rect.right(); x += gridSizeX)
            lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        for (qreal y = top; y < rect.bottom(); y += gridSizeY)
            lines.append(QLineF(rect.left(), y, rect.right(), y));


        painter->setPen(QPen(QBrush(Qt::darkGray),0,Qt::DashDotDotLine));

        painter->drawLines(lines.data(), lines.size());

        painter->setPen(QPen(QBrush(Qt::lightGray),0,Qt::DashLine));
        painter->drawLine(QLineF(16.66, rect.top(), 16.66, rect.bottom()));
        painter->drawLine(QLineF(33.33, rect.top(), 33.33, rect.bottom()));
    }
};

PerformanceDialog::PerformanceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerformanceDialog),
    mFirstVSync(true),
    mCurrentRow(0)
{
    ui->setupUi(this);
    mScene = new GridScene;
    mScene->setBackgroundBrush(QBrush(Qt::black));
    mScene->setSceneRect(0,0,33.333,100);
    ui->graphicsView->setScene(mScene);

    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
//    ui->graphicsView->viewport()->installEventFilter(this);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->pauseButton->setEnabled(false);

    mUpdateValues = new CircularBuffer(60);
    mRenderValues = new CircularBuffer(60);
    mSwapValues = new CircularBuffer(60);
    QObject::connect(this,SIGNAL(messageAvailable()),this,SLOT(processMessages()), Qt::QueuedConnection);

}

PerformanceDialog::~PerformanceDialog()
{
    delete mScene;
    delete ui;
    delete mUpdateValues;
    delete mRenderValues;
    delete mSwapValues;
}

void PerformanceDialog::setConnection(QString hostAddress, quint16 portNumber)
{
    mHostAddress = hostAddress;
    mPortNumber = portNumber;
}

void PerformanceDialog::resizeEvent(QResizeEvent * /* event */)
{
    ui->graphicsView->fitInView(0,KLineSize*mCurrentRow-ui->graphicsView->height(),33.33,ui->graphicsView->height());
}

//bool PerformanceDialog::eventFilter(QObject *obj, QEvent *event)
//{
//    if (obj == ui->graphicsView->viewport()) {
//        return false;
//    } else {
//        return QDialog::eventFilter(obj, event);
//    }
//}

void PerformanceDialog::MessageReceived(QString& recv)
{
    QMutexLocker lock(&mMessagesMutex);
    mMessages.push_back(recv);
    if (mMessages.size()==1) {
        // only ever signal on 0 -> 1
        emit messageAvailable();
    }
}

void PerformanceDialog::processMessages()
{
    //qWarning() << recv;
    bool messagesAvailable = true;
    int maxMessageCount=0;
    while (messagesAvailable)
    {
        QString recv;
        {
            QMutexLocker lock(&mMessagesMutex);
            int numMessages = mMessages.size();
            if (numMessages > maxMessageCount)
            {
                maxMessageCount = numMessages;
            }
            if (numMessages >0) {
                recv = mMessages.front();
                mMessages.pop_front();
            } else {
                messagesAvailable = false;
            }
        }
        if (messagesAvailable) {
            QStringList args = recv.split(" ");
            if (args.length()>=3) {
                double time = args[0].toDouble();
                if (args[2].compare("V_SYNC")==0) {
                    if (!mFirstVSync) {
                        double vsyncduration = (time - mLastVSync);
                        mLastVSync = time;
                        if (vsyncduration < 0.0333) {
                            addVSync(vsyncduration);
                            mCurrentRow++;
                        } else {
                            mCurrentRow+=5;
                            mUpdateStart = 0;
                            mRenderStart = 0;
                            mSwapStart = 0;
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
                        if (mUpdateStart != 0)
                            addUpdate();
                    } else if (args[2].compare("RENDER_START")==0) {
                        mRenderStart = time;
                        mRenderVSync = mLastVSync;
                        mRenderRow = mCurrentRow;
                    } else if (args[2].compare("RENDER_END")==0) {
                        mRenderEnd = time;
                        if (mRenderStart != 0)
                            addRender();
                    } else if (args[2].compare("SWAP_START")==0) {
                        mSwapStart = time;
                        mSwapVSync = mLastVSync;
                        mSwapRow = mCurrentRow;
                    } else if (args[2].compare("SWAP_END")==0) {
                        mSwapEnd = time;
                        if (mSwapStart != 0)
                         addSwapBuffers();
                    }
                }
            }
        }
    }
    double verticalSize = KLineSize * mCurrentRow;
    mScene->setSceneRect(0,0,33.33,verticalSize);
    ui->graphicsView->fitInView(0,verticalSize-ui->graphicsView->height(),33.33,ui->graphicsView->height());

    double update_avg = mUpdateValues->average();
    double render_avg = mRenderValues->average();
    double swap_avg = mSwapValues->average();
    double update_max = mUpdateValues->maxValue();
    double render_max = mRenderValues->maxValue();
    double swap_max = mSwapValues->maxValue();
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(QString::number(update_avg)));
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(QString::number(render_avg)));
    ui->tableWidget->setItem(2,1,new QTableWidgetItem(QString::number(swap_avg)));
    ui->tableWidget->setItem(3,1,new QTableWidgetItem(QString::number(update_avg+render_avg+swap_avg)));

    ui->tableWidget->setItem(0,2,new QTableWidgetItem(QString::number(update_max)));
    ui->tableWidget->setItem(1,2,new QTableWidgetItem(QString::number(render_max)));
    ui->tableWidget->setItem(2,2,new QTableWidgetItem(QString::number(swap_max)));

    if (maxMessageCount>32)
    {
        qDebug() << "message count > 32 " << maxMessageCount;
    }
}



void PerformanceDialog::addBar(double startTime, double duration, double row, QColor startCol, QColor midCol, QColor endCol)
{
    QLinearGradient gradient(startTime,0,startTime+duration, 0);
    gradient.setColorAt(0, startCol);
    gradient.setColorAt(0.5, midCol);
    gradient.setColorAt(1.0, endCol);
    if (startTime + duration > 33.333) {
        duration = 33.333-startTime;
    }
    mScene->addRect(QRectF(startTime,row*(double)KLineSize, duration, KLineDrawSize), Qt::NoPen, QBrush(gradient));
}

void PerformanceDialog::addUpdate()
{
    double startTime = (mUpdateStart-mUpdateVSync)*KScaleX;
    double duration= (mUpdateEnd-mUpdateStart)*KScaleX;
    mUpdateValues->addValue(duration);

    addBar(startTime,duration, mUpdateRow, QColor(180,255,0), QColor(120,255,0),QColor(0,255,0));

    if (mCurrentRow!=mUpdateRow) {
        startTime = 0;
        duration = (mUpdateEnd-mLastVSync)*KScaleX;
        addBar(startTime,duration, mCurrentRow +0.5, QColor(120,255,0), QColor(120,255,0),QColor(0,255,0));
    }
}


void PerformanceDialog::addRender()
{
    double startTime = (mRenderStart-mRenderVSync)*KScaleX;
    double duration= (mRenderEnd-mRenderStart)*KScaleX;
    mRenderValues->addValue(duration);

    addBar(startTime,duration, mRenderRow +0.5, QColor(0,180,255), QColor(0,120,255),QColor(0,0,255));

    if (mCurrentRow!=mRenderRow) {
        startTime = 0;
        duration = (mRenderEnd-mLastVSync)*KScaleX;
        addBar(startTime,duration, mCurrentRow +0.5, QColor(0,120,255), QColor(0,120,255),QColor(0,0,255));
    }
}

void PerformanceDialog::addSwapBuffers()
{
    double startTime = (mSwapStart-mSwapVSync)*KScaleX;
    double duration= (mSwapEnd-mSwapStart)*KScaleX;
    mSwapValues->addValue(duration);

    addBar(startTime,duration, mSwapRow +0.5, QColor(255,255,180), QColor(255,255,120),QColor(255,255,0));

    if (mCurrentRow!=mSwapRow) {
        startTime = 0;
        duration = (mSwapEnd-mLastVSync)*KScaleX;
        addBar(startTime,duration, mCurrentRow +0.5, QColor(255,255,120), QColor(255,255,120),QColor(255,255,0));
    }
}

void PerformanceDialog::addVSync(double time)
{
    double xpos = time * KScaleX;
    mScene->addLine(QLineF(xpos, mCurrentRow*KLineSize, xpos, mCurrentRow*KLineSize+KLineSize), QPen(Qt::red,0));
}

void PerformanceDialog::pause()
{
    mClient.sendCommand(KDaliCmdPerformanceMarkersOff);
    ui->pauseButton->setEnabled(false);
    ui->startButton->setEnabled(true);
}

void PerformanceDialog::start()
{
    mFirstVSync = true;
    mClient.connectSocket(mHostAddress, mPortNumber);
    mClient.sendCommand(KDaliCmdPerformanceMarkersOn);
    mClient.waitForMessages(this);
    ui->pauseButton->setEnabled(true);
    ui->startButton->setEnabled(false);
}
