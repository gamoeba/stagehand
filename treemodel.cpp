#include "treemodel.h"

#include <QStandardItemModel>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QApplication>
#include <jsonitem.h>
#include "nodeobject.h"
#include "mainwindow.h"

TreeModel::TreeModel(QObject *parent) :
    QObject(parent),
    settings(MainWindow::settings)
{
    mModel = new QStandardItemModel;
}

void TreeModel::setTreeData(QJsonDocument doc)
{
    jsonDoc = doc;
    mModel->clear();
    QJsonObject obj = jsonDoc.object();
    QJsonValue value = obj.value(settings.mNodeName);
    QJsonValue id = obj.value(settings.mNodeID);
    QStandardItem* itemx = new JsonItem(obj);
    mModel->appendRow(itemx);
    mIndices[id.toInt()] = itemx;
    QJsonValue children = obj.value(settings.mNodeChildrenName);
    if (children.isArray()) {
        addChildren(itemx, children.toArray());
    }
}

void TreeModel::addChildren(QStandardItem* parent, QJsonArray array) {
    QJsonArray::iterator iter;

    for (iter = array.begin(); iter !=array.end();iter++) {
        QJsonValue val = *iter;
        if (val.isObject()) {
            QJsonObject obj = val.toObject();
            QJsonValue value = obj.value(settings.mNodeName);
            QJsonValue id = obj.value(settings.mNodeID);
            QStandardItem* itemx = new JsonItem(obj);
            mIndices[id.toInt()] = itemx;
            parent->appendRow(itemx);
            QJsonValue children = obj.value(settings.mNodeChildrenName);
            if (children.isArray()) {
                addChildren(itemx, children.toArray());
            }
        }
    }
}

