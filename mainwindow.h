/****************************************************************************
Copyright (C) 2014 Gamoeba Ltd
All rights reserved

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "treemodel.h"
#include "tablemodel.h"
#include "settings.h"
#include "delegate.h"

class GLWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setHostName(QString hostName);
    void setPortNumber(QString portNumber);

    QJsonDocument readJson(QString &fileName);

    static Settings settings;

public slots:
    void loadFile();
    void saveFile();
    void zoomIn();
    void zoomOut();
    void updateScene();
    void selectedId(int id);
    void showScreenShot(bool show);
    void tableItemChanged(QStandardItem*);

protected:
    void inputFiles(QString jsonFile, QString screenShotFile);
    void updateTableView(const QModelIndex &index);
    void updateGLView(const QModelIndex &index);
private slots:
    void on_treeView_clicked(const QModelIndex &index);


    void on_clearButton_clicked();

    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    TreeModel* mTreeModel;
    TableModel* mTableModel;
    GLWidget* mGLWidget;

    QJsonDocument mDoc;

    void addObjects();
    void addObjects2(QJsonArray array);
    bool addNodeObject(QJsonObject obj);
};

#endif // MAINWINDOW_H
