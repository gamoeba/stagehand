#ifndef PERFORMANCEDIALOG_H
#define PERFORMANCEDIALOG_H

#include <QDialog>

namespace Ui {
class PerformanceDialog;
}

class PerformanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PerformanceDialog(QWidget *parent = 0);
    ~PerformanceDialog();

    void setConnection(QString hostAddress, int portNumber);

private:
    Ui::PerformanceDialog *ui;
};

#endif // PERFORMANCEDIALOG_H
