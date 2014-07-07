#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <map>

#include "settings.h"

class TreeModel : public QObject
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = 0);

    void setTreeData(QJsonDocument doc);

    QStandardItemModel* model() {return mModel;}
    void addChildren(QStandardItem* parent, QJsonArray array);

    QModelIndex getIndex(int id) {return mIndices[id]->index();}
signals:

public slots:

private:
    QStandardItemModel* mModel;
    QJsonDocument jsonDoc;
    Settings& settings;
    std::map<int, QStandardItem*> mIndices;
};

#endif // TREEMODEL_H
