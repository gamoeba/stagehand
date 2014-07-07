#include "tablemodel.h"
#include "dataobject.h"

#include <QStandardItemModel>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <jsonitem.h>
#include <QTableWidget>
#include <QHeaderView>

#include "nodeobject.h"

TableModel::TableModel(QTableView* table) :
    QObject(NULL), mTable(table)
{
    mModel = new QStandardItemModel;
}

void TableModel::setTableData(QJsonObject obj)
{
    mJsonObject = obj;

    QJsonValue value = obj.value(QString("Name"));
    QJsonValue id = obj.value(QString("id"));

    QJsonValue children =  obj.value(QString("properties"));
    if (children.isArray()) {
        addChildren(children.toArray());
    }

}

void TableModel::addChildren( QJsonArray array) {
    QJsonArray::iterator iter;

    for (iter = array.begin(); iter !=array.end();iter++) {
        QJsonValue val = *iter;
        if (val.isArray()) {
            QJsonArray arr = val.toArray();
            QString valStr = arr.at(1).toString();
            valStr.replace(QRegExp("\\] *, *\\["),"],\n[");
            QStandardItem* name = new QStandardItem(arr.at(0).toString());
            QStandardItem* value = new QStandardItem(valStr);
            QList<QStandardItem*> items;
            items.append(name);
            items.append(value);
            mModel->appendRow(items);

            DataObject obj(arr.at(1).toString());
            obj.isMatrix();


//            QModelIndex mi = mModel->indexFromItem(value);

//            QTableWidget* tw = new QTableWidget;
//            tw->setRowCount(4);
//            tw->setColumnCount(4);
//            tw->horizontalHeader()->setVisible(false);
//            tw->verticalHeader()->setVisible(false);
//            mTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
//            //mTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
//            mTable->verticalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
//            //mTable->verticalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

//            mTable->setIndexWidget(mi, tw);

        }
    }
}

