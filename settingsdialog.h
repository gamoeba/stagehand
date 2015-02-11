#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void updateFromSettings(Settings &settings);
    void updateSettings(Settings &settings);
public slots:
    void cellEntered(int row, int column);
private:
    QStringList mEditableSettings;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
