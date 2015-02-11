#include "performancedialog.h"
#include "ui_performancedialog.h"
#include "socketclient.h"
#include "consts.h"

PerformanceDialog::PerformanceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerformanceDialog)
{
    ui->setupUi(this);
}

PerformanceDialog::~PerformanceDialog()
{
    delete ui;
}

void PerformanceDialog::setConnection(QString hostAddress, int portNumber)
{
    SocketClient client;
    client.sendCommand(hostAddress,portNumber,KDaliCmdPerformanceMarkersOn);
}

