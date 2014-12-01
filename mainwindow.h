/*
 * Copyright (c) 2014 Gamoeba Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <QLabel>
#include <QSplashScreen>
#include <QTreeView>
#include "treemodel.h"
#include "tablemodel.h"
#include "settings.h"
#include "delegate.h"
#include "filedownloader.h"

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

    void setAppNameAndDirectory(QString appName, QDir directory);
    void setHostName(QString hostName);
    void setPortNumber(QString portNumber);

    QJsonDocument readJson(const QString &fileName);

    static Settings settings;

    void portForward();
    void nextSelection();
    void saveJson(QString &fileName, const QString &str);
    void takeScreenShot();

signals:
    void newImage(QImage img);
public slots:
    void loadFile();
    void saveFile();
    void zoomIn();
    void zoomOut();
    void updateScene();
    void selectedId(int id);
    void mousePositionChanged(int x, int y);
    void showScreenShot(bool show);
    void tableItemChanged(QStandardItem*);
    void newImageReceived(QImage img);
    void aboutStagehand();

protected:
    void updateTableView(const QModelIndex &index);
    void updateGLView(const QModelIndex &index);
    void refreshScene();
private slots:
    void on_treeView_clicked(const QModelIndex &index);


    void on_clearButton_clicked();

    void on_sendButton_clicked();

    void on_treeSearch_textEdited(const QString &strSearch);

    void on_treeSearch_editingFinished();

    void on_treeSearch_returnPressed();

private:
    Ui::MainWindow *ui;
    TreeModel* mTreeModel;
    TableModel* mTableModel;
    GLWidget* mGLWidget;

    QString mCurrentTreeSearch;
    QString mJsonTxt;
    QModelIndexList mCurrentTreeSearchResults;
    int mCurrentTreeSearchIndex;

    QJsonDocument mDoc;
    QLabel* mSBLabel;
    FileDownloader* m_pDownloader;
    bool mChangingProperties;
    void addObjects();
    void addObjects2(QJsonArray array);
    bool addNodeObject(QJsonObject obj);
    void resetSearch();
    void GetExpandedState(QTreeView *view, QStandardItemModel *model, std::map<int, bool>& expandedState, int& selectedIndex, int &treePosition);
    void RestoreExpandedState(QTreeView* view, QStandardItemModel * model, std::map<int, bool>& expandedState, int selectedIndex, int treePosition);
};

#endif // MAINWINDOW_H
