#include <QFileDialog>
#include "initialsettingsdialog.h"
#include "ui_initialsettingsdialog.h"
#include "settings.h"

InitialSettingsDialog::InitialSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitialSettingsDialog)
{
    ui->setupUi(this);
}

InitialSettingsDialog::~InitialSettingsDialog()
{
    delete ui;
}

void InitialSettingsDialog::updateFromSettings(Settings& settings) {
    ui->hostNameEdit->setText(settings[KHostName]);

    if (settings[KForwardMode]=="direct") {
        ui->forwardMode->setCurrentIndex(0);
        ui->forwardStringEdit->setEnabled(false);
    } else {
        ui->forwardMode->setCurrentIndex(1);
        ui->hostNameEdit->setEnabled(false);
    }
    ui->portNumberEdit->setText(settings[KPortNumber]);
    ui->forwardStringEdit->setText(settings[KForwardPortDest]);
    ui->pathEdit->setText(settings[KTargetToolsPath]);
    int index = ui->targetPlatformSelect->findText(settings[KTargetType]);
    if (index != -1) {
        ui->targetPlatformSelect->setCurrentIndex(index);
    }
    int colourScheme = ui->colourSchemeSelect->findText(settings[KColourScheme]);
    if (colourScheme != -1) {
        ui->colourSchemeSelect->setCurrentIndex(colourScheme);
    }
}

void InitialSettingsDialog::updateSettings(Settings& settings) {
    settings[KHostName] = ui->hostNameEdit->text();
    settings[KPortNumber] = ui->portNumberEdit->text();
    settings[KForwardPortDest] = ui->forwardStringEdit->text();
    settings[KTargetToolsPath] = ui->pathEdit->text();
    settings[KTargetType] = ui->targetPlatformSelect->currentText();
    settings[KColourScheme] = ui->colourSchemeSelect->currentText();
    if (ui->forwardMode->currentIndex()==0) {
        settings[KForwardMode] = "direct";
    } else {
        settings[KForwardMode] = "forward";
    }
    settings.saveSettings();
}

void InitialSettingsDialog::modeChanged(int index) {
  if (index==0) {
    ui->forwardStringEdit->setEnabled(false);
    ui->hostNameEdit->setEnabled(true);
  } else {
    ui->forwardStringEdit->setEnabled(true);
    ui->hostNameEdit->setEnabled(false);
  }
}

void InitialSettingsDialog::pathBrowse()
{
    QString dir = ui->pathEdit->text();
    QString newDir = QFileDialog::getExistingDirectory(this,"choose directory",dir);
    if (newDir.length()>0)
        ui->pathEdit->setText(newDir);
}
