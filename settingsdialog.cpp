#include <QMap>
#include <QString>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

}


void SettingsDialog::updateFromSettings(Settings& settings) {
    QMap<QString, QString>& allSettings = settings.getSettings();
    QMap<QString, QString>::iterator iter;
    int numRows = allSettings.size();
    ui->tableWidget->setRowCount(numRows);
    int row = 0;
    for (iter = allSettings.begin();iter !=allSettings.end();++iter)
    {
        QTableWidgetItem *name = new QTableWidgetItem(iter.key());
        name->setFlags(name->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *value = new QTableWidgetItem(iter.value());
        ui->tableWidget->setItem(row, 0, name);
        ui->tableWidget->setItem(row, 1, value);
        ++row;
    }
}

void SettingsDialog::updateSettings(Settings& settings) {
    int rowCount = ui->tableWidget->rowCount();
    for (int row = 0; row < rowCount;++row)
    {
        QTableWidgetItem* name = ui->tableWidget->item(row,0);
        QTableWidgetItem* value = ui->tableWidget->item(row,1);
        settings[name->text()] = value->text();
    }
    settings.saveSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::cellEntered(int /*row*/, int /*column*/)
{
}
