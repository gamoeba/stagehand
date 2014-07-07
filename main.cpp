#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (argc>1) {
        QString hostName(argv[1]);
        w.setHostName(hostName);
    }
    if (argc ==3) {
        QString port(argv[2]);
        w.setPortNumber(port);
    }
    w.show();

    return a.exec();
}
