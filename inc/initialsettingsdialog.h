#ifndef INITIALSETTINGSDIALOG_H
#define INITIALSETTINGSDIALOG_H

#include <QDialog>
#include "settings.h"

namespace Ui {
class InitialSettingsDialog;
}

class InitialSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitialSettingsDialog(QWidget *parent = 0);
    ~InitialSettingsDialog();

    void updateFromSettings(Settings &settings);
    void updateSettings(Settings &settings);
public slots:
    void modeChanged(int index);
    void pathBrowse();
private:
    Ui::InitialSettingsDialog *ui;
};

#endif // INITIALSETTINGSDIALOG_H
