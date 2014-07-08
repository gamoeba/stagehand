#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "treemodel.h"
#include "tablemodel.h"
#include "settings.h"

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
