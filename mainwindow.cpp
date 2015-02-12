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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jsonitem.h"
#include "glwidget.h"
#include "nodeobject.h"
#include "sceneobject.h"
#include "socketclient.h"
#include "performancedialog.h"

#include <QStandardItemModel>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QTableWidget>
#include <QSettings>
#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QtConcurrent/QtConcurrent>
#include "stagehandarchive.h"
#include "version.h"
#include "utils.h"
#include "settingsdialog.h"
#include "consts.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mChangingProperties(false)
{
    QDesktopWidget desktop;
    QRect screenSize = desktop.availableGeometry(this);
    QSize sz(screenSize.width() * 0.8f, screenSize.height() * 0.8f);

    ui->setupUi(this);

    mGLWidget = new GLWidget();

    mGLWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    mTreeModel = new TreeModel;
    mTableModel = new TableModel(ui->tableView);

    ui->treeView->setAlternatingRowColors(false);
    ui->updatePanel->setHidden(true);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->treeView->header()->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->header()->setStretchLastSection(false);
    ui->treeView->setHeaderHidden(true);

    ui->tableViewUpdate->setModel(new QStandardItemModel());
    ui->tableViewUpdate->setAlternatingRowColors(true);
    ui->tableViewUpdate->horizontalHeader()->setVisible(false);
    ui->tableViewUpdate->verticalHeader()->setVisible(false);

    TableDelegate* delegate = new TableDelegate(1);
    ui->tableView->setItemDelegateForColumn(1, delegate);
    TableDelegate* delegateUpdate = new TableDelegate(2);
    ui->tableViewUpdate->setItemDelegateForColumn(2 ,delegateUpdate);

    QObject::connect(mTableModel->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(tableItemChanged(QStandardItem*)));
    QObject::connect(mGLWidget, SIGNAL(selectedId(int)), this, SLOT(selectedId(int)));
    QObject::connect(mGLWidget, SIGNAL(mousePosition(int,int)), this, SLOT(mousePositionChanged(int,int)));
    QObject::connect(this, SIGNAL(newImage(QImage)), this, SLOT(newImageReceived(QImage)));

    mSBLabel = new QLabel();
    ui->statusBar->addWidget(mSBLabel);
    ui->glParent->addWidget(mGLWidget);

    QFont font;
    int pointSize = settings[KFontPointSize].toInt();
    font.setPointSize(pointSize);
    ui->tableView->setFont(font);
    ui->tableViewUpdate->setFont(font);
    ui->treeView->setFont(font);

    resize(sz);
    this->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mGLWidget;
    delete mTreeModel;
    delete mTableModel;
}

void MainWindow::setHostName(QString hostName)
{
    settings[KHostName] = hostName;
}

void MainWindow::setPortNumber(QString portNumber)
{
    settings[KPortNumber] = portNumber;
}

void MainWindow::loadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.zip)"),NULL,
                                                     QFileDialog::DontUseNativeDialog);

    StagehandArchive sa;
    sa.unzip(fileName);
    mJsonTxt = sa.getSceneData();
    mDoc = QJsonDocument::fromJson(mJsonTxt.toUtf8());
    mGLWidget->setScreenShot(sa.getScreenShot());
    refreshScene();
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                   "",
                                                   tr("Files (*.zip)"),NULL,
                                                   QFileDialog::DontUseNativeDialog);

    StagehandArchive sa;
    sa.setSceneData(mJsonTxt);
    sa.setScreenShot(mGLWidget->getScreenShot());
    sa.zip(fileName);
}

void MainWindow::zoomIn()
{
    mGLWidget->zoomIn();
}

void MainWindow::zoomOut()
{
    mGLWidget->zoomOut();
}

void MainWindow::updateScene()
{
    portForward();
    SocketClient client;
    client.connectSocket(settings[KHostName], settings[KPortNumber].toUInt());
    mJsonTxt = client.sendCommandSizedReturn( KDaliCmdDumpScene);

    if (mJsonTxt.length() > 0 ) {
        mDoc = QJsonDocument::fromJson(mJsonTxt.toUtf8());

        QImage img;
        mGLWidget->setScreenShot(img); // set an empty image through to clear any previous images while we wait for the screenshot
        QtConcurrent::run(this, &MainWindow::takeScreenShot);
        refreshScene();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Error");
        msgBox.setInformativeText("Connection to device failed");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::refreshScene()
{
    if (! (mDoc.isNull() || mDoc.isEmpty()) )
    {
        std::map<int,bool> expandedState;
        int selectedIndex, currentIndex;
        int treePosition;
        GetExpandedState(ui->treeView, mTreeModel->model(), expandedState, selectedIndex, currentIndex, treePosition);
        mTreeModel->setTreeData(mDoc);
        ui->treeView->setModel(mTreeModel->model());

        QObject::connect(ui->treeView->selectionModel(),
                         SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&)),
                         this,
                         SLOT(treeCurrentItemChanged(const QModelIndex&, const QModelIndex&)));

        RestoreExpandedState(ui->treeView, mTreeModel->model(),expandedState, selectedIndex, currentIndex, treePosition);
        //ui->treeView->expandAll();
        addObjects();
        mGLWidget->repaint();
        resetSearch();
    }
}

void MainWindow::selectedId(int id)
{
   QModelIndex index = mTreeModel->getIndex(id);
   ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect) ;
   updateTableView(index);
}

void MainWindow::mousePositionChanged(int x, int y)
{
    QString lb = "Cursor Pos: ";
    if (x == -1 && y == -1) {
        lb += "Offscreen";
    } else {
        lb += "( %1 , %2 )";
        lb = lb.arg(QString::number(x),QString::number(y));
    }
    mSBLabel->setText(lb);
}

void MainWindow::showScreenShot(bool show)
{
    mGLWidget->showScreenShot(show);
}

void MainWindow::addObjects() {
    mGLWidget->clear();
    QJsonObject obj = mDoc.object();
    addNodeObject(obj);
    QJsonValue children = obj.value(KDaliNodeChildrenName);
    if (children.isArray()) {
        QJsonArray array = children.toArray();
        addObjects2(array);
    }
}

bool MainWindow::addNodeObject(QJsonObject obj) {
    QJsonValue value = obj.value(KDaliNodeName);
    QJsonValue id = obj.value(KDaliNodeId);
    int idVal = (int)id.toInt();

    NodeObject node(obj, KDaliNodePropertiesName);
    bool vis = node.getProperty(KDaliNodeVisible).toBoolean();
    if (value.toString() == KDaliCameraNodeName) {
        DataObject pm = node.getProperty(KDaliCameraNodeProjectionMatrixName);
        mGLWidget->setProjectionMatrix(pm.get4x4Matrix());
        DataObject vm = node.getProperty(KDaliCameraNodeViewMatrixName);
        mGLWidget->setViewMatrix(vm.get4x4Matrix());

        double aspectRatio = node.getProperty(KDaliCameraNodeAspectRatioName).toDouble();
        mGLWidget->setAspectRatio(aspectRatio);
    } else {
        QMatrix4x4 wm = node.getProperty(KDaliNodeWorldMatrixName).get4x4Matrix();
        std::vector<double> size = node.getProperty(KDaliNodeSizeName).getVector();
        QVector3D qsize(size[0], size[1], size[2]);
        if (vis) {
            mGLWidget->addObject(idVal, SceneObject(wm, qsize));
        }
    }
    return vis;
}

void MainWindow::addObjects2(QJsonArray array) {
    QJsonArray::iterator iter;
    QMatrix4x4 projectionMatrix, viewMatrix;


    for (iter = array.begin(); iter !=array.end();iter++) {
        QJsonValue val = *iter;
        if (val.isObject()) {
            QJsonObject obj = val.toObject();
            bool vis = addNodeObject(obj);
            QJsonValue children = obj.value(KDaliNodeChildrenName);
            if (vis && children.isArray()) {
                addObjects2(children.toArray());
            }
        }
    }
}


QJsonDocument MainWindow::readJson(const QString& fileName)
   {
      QFile file;
      file.setFileName(fileName);
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      mJsonTxt = file.readAll();
      file.close();

      QJsonDocument d = QJsonDocument::fromJson(mJsonTxt.toUtf8());
      return d;
   }

void MainWindow::saveJson(QString& fileName, const QString& str)
{
    QFile file;
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    file.write(str.toUtf8());
    file.close();
}

void MainWindow::updateGLView(const QModelIndex &index)
{
    QVariant var = ui->treeView->model()->data(index, JsonItem::JsonRole);
    QJsonObject obj = var.toJsonObject();
    int id = obj.value(KDaliNodeId).toInt();
    mGLWidget->setSelection(id);
}

void MainWindow::updateTableView(const QModelIndex &index)
{
    QVariant var = ui->treeView->model()->data(index, JsonItem::JsonRole);
    QJsonObject obj = var.toJsonObject();
    mChangingProperties = true;
    mTableModel->setTableData(obj);
    mChangingProperties = false;
    ui->tableView->setModel(mTableModel->model());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

}

void MainWindow::GetExpandedState(QTreeView* view,
                                  QStandardItemModel* model,
                                  std::map<int, bool>& expandedState,
                                  int& selectedIndex,
                                  int& currentIndex,
                                  int& treePosition)
{

    QModelIndexList children;

    //  Get top-level first.
    for ( int i = 0; i < model->rowCount(); ++i ) {
        children << model->index( i, 0 );  //  Use whatever column you are interested in.
    }

    // Now descend through the generations.
    for ( int i = 0; i < children.size(); ++i ) {
        for ( int j = 0; j < model->rowCount( children[i] ); ++j ) {
            children << children[i].child( j, 0 );
        }
    }

    foreach (QModelIndex child, children)
    {
        QVariant var = model->itemFromIndex(child)->data(JsonItem::IdRole);
        int index = var.toInt();
        expandedState[index] = view->isExpanded(child);
    }

    selectedIndex = 0;
    currentIndex = 0;
    QItemSelectionModel *sel = view->selectionModel();
    if (sel) {
        QModelIndexList modelindexlist = sel->selection().indexes();

        if (!modelindexlist.empty()) {
            QModelIndex ind = modelindexlist.at(0);
            selectedIndex = model->itemFromIndex(ind)->data(JsonItem::IdRole).toInt();
        }
        QModelIndex curr = sel->currentIndex();
        QStandardItem* currItem = model->itemFromIndex(curr);
        if (currItem)
            currentIndex = currItem->data(JsonItem::IdRole).toInt();
    }
    QModelIndex ind = view->indexAt(view->rect().topLeft());
    QStandardItem* item = model->itemFromIndex(ind);
    if (item != NULL) {
        treePosition = item->data(JsonItem::IdRole).toInt();
    }

}

void MainWindow::RestoreExpandedState(QTreeView *view, QStandardItemModel *model, std::map<int, bool> &expandedState, int selectedIndex, int currentIndex, int treePosition)
{
    QModelIndexList children;

    //  Get top-level first.
    for ( int i = 0; i < model->rowCount(); ++i ) {
        children << model->index( i, 0 );  //  Use whatever column you are interested in.
    }

    // Now descend through the generations.
    for ( int i = 0; i < children.size(); ++i ) {
        for ( int j = 0; j < model->rowCount( children[i] ); ++j ) {
            children << children[i].child( j, 0 );
        }
    }
    foreach (QModelIndex child, children)
    {
        QVariant var = model->itemFromIndex(child)->data(JsonItem::IdRole);
        int index = var.toInt();
        if (expandedState[index]) {
            view->expand(child);
        }
        if (index == selectedIndex)
        {
            view->selectionModel()->select(child, QItemSelectionModel::ClearAndSelect);
        }

        if (index == currentIndex)
        {
            view->selectionModel()->setCurrentIndex(child, QItemSelectionModel::Current);
            updateTableView(child);
            updateGLView(child);
        }

        if (index == treePosition) {
            view->scrollTo(child, QAbstractItemView::PositionAtTop);
        }
    }

}

void MainWindow::tableItemChanged(QStandardItem * item)
{
    if (!mChangingProperties)
    {
        QString valueStr = item->data(Qt::DisplayRole).toString();
        QModelIndex ind = item->index();
        QString nameStr =  mTableModel->model()->itemFromIndex(ind.sibling(ind.row(),0))->data(Qt::DisplayRole).toString();
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableViewUpdate->model());
        int idval = mTableModel->getObjectId();
        int rows = model->rowCount();
        bool found = false;
        for (int i=0;i<rows &&!found;i++) {

            int currid = model->data(model->index(i,0), Qt::DisplayRole).toInt();
            QString currname = model->data(model->index(i,1), Qt::DisplayRole).toString();
            if (currid==idval && currname == nameStr) {
                found = true;
                //model->setData(model->index(i,2), QVariant(valueStr),Qt::DisplayRole);
                // SetItem force repaint but for some reason setData doesn't?
                QStandardItem* value = new QStandardItem(valueStr);
                model->setItem(i,2, value);
            }
        }
        if (!found) {
            QStandardItem* id = new QStandardItem(QString::number(idval));
            id->setFlags(id->flags() ^ Qt::ItemIsEditable);
            QStandardItem* name = new QStandardItem(nameStr);
            name->setFlags(name->flags() ^ Qt::ItemIsEditable);
            QStandardItem* value = new QStandardItem(valueStr);
            QList<QStandardItem*> items;
            items.append(id);
            items.append(name);
            items.append(value);
            model->appendRow(items);
        }
        ui->updatePanel->setHidden(false);
        ui->tableViewUpdate->resizeColumnsToContents();
        ui->tableViewUpdate->resizeRowsToContents();
    }
}

void MainWindow::newImageReceived(QImage img)
{
    mGLWidget->setScreenShot(img);
    mGLWidget->repaint();
}

void MainWindow::aboutStagehand()
{
    QMessageBox msgBox;
    QString path = QApplication::applicationDirPath();

    msgBox.setText("Stage hand");
    msgBox.setInformativeText("Version: " + QString::number(STAGEHAND_VERSION));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_clearButton_clicked()
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableViewUpdate->model());
    model->clear();
    ui->updatePanel->setHidden(true);
}

void MainWindow::on_sendButton_clicked()
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableViewUpdate->model());
    int rows = model->rowCount();
    QString command = " |";
    for (int i=0;i<rows;i++) {
        int currid = model->data(model->index(i,0), Qt::DisplayRole).toInt();
        QString currname = model->data(model->index(i,1), Qt::DisplayRole).toString();
        QString currvalue = model->data(model->index(i,2), Qt::DisplayRole).toString();
        command += QString::number(currid) + ";" + currname + ";" + currvalue;
        if (i!= rows -1) {
            command += "|";
        }
    }
    portForward();
    SocketClient client;
    client.connectSocket(settings[KHostName], settings[KPortNumber].toUInt());
    client.sendCommand( KDaliCmdSetProperties + command);

    // after sending, clear the data
    on_clearButton_clicked();
}


void MainWindow::nextSelection() {
    if (mCurrentTreeSearchResults.size()>0) {
        const QModelIndex& index = mCurrentTreeSearchResults[mCurrentTreeSearchIndex];
        ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
        updateTableView(index);
        updateGLView(index);

        mCurrentTreeSearchIndex++;
        mCurrentTreeSearchIndex %= mCurrentTreeSearchResults.size();
    }

}

void MainWindow::on_treeSearch_textEdited(const QString &strSearch)
{
    if (strSearch.size()>0) {
        if (mCurrentTreeSearch != strSearch) {
            mCurrentTreeSearchResults = mTreeModel->search(strSearch);
            mCurrentTreeSearchIndex = 0;
            mCurrentTreeSearch = strSearch;
        }
        nextSelection();
    }
}

void MainWindow::resetSearch()
{
    if (mCurrentTreeSearch.length()>0) {
        mCurrentTreeSearchResults = mTreeModel->search(mCurrentTreeSearch);
        mCurrentTreeSearchIndex = 0;
        nextSelection();
    } else {
        mCurrentTreeSearchResults.clear();
        mCurrentTreeSearchIndex = 0;
    }
}

void MainWindow::on_treeSearch_editingFinished()
{
}

void MainWindow::on_treeSearch_returnPressed()
{
    nextSelection();
}

void MainWindow::treeCurrentItemChanged(const QModelIndex& current, const QModelIndex& /*previous*/)
{
    updateTableView(current);
    updateGLView(current);
}


void MainWindow::takeScreenShot()
{
    QProcess process;
    QString screenShotFile = appendPath(QDir::tempPath(), QString("screenshot.png"));

    runPlatformScript("takescreenshot", QStringList() << screenShotFile);

    QImage img(screenShotFile);
    QFile::remove(screenShotFile);
    // notify main thread since we can only update GL widget on main thread
    emit newImage(img);
}

void MainWindow::portForward() {
    if (settings[KForwardPortDest].length()>0) {
        runPlatformScript("portforward", QStringList() << settings[KPortNumber] << settings[KForwardPortDest]);
    }
}

void MainWindow::editSettings()
{
    SettingsDialog d;
    d.updateFromSettings(settings);
    int res = d.exec();
    if (res == QDialog::Accepted) {
        d.updateSettings(settings);
    }
}

void MainWindow::performanceViewer()
{
    PerformanceDialog p;
    p.setConnection(settings[KHostName], settings[KPortNumber].toUInt());
    p.exec();

}

void MainWindow::runPlatformScript(QString scriptName, QStringList parameters)
{

    QString scriptlocation = appendPath(qApp->applicationDirPath(), settings[KTargetType]);
    QString scriptfullpath = appendPath(scriptlocation, scriptName);
    qDebug() << "running " << scriptfullpath;

    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", env.value("Path") + ":" + settings[KTargetToolsPath]);
    process.setProcessEnvironment(env);

    process.execute(scriptfullpath, parameters);
    process.waitForFinished();
}
