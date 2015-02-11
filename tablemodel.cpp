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
#include "consts.h"

TableModel::TableModel(QTableView* table) :
    QObject(NULL), mTable(table)
{
    mModel = new QStandardItemModel;
}

void TableModel::setTableData(QJsonObject obj)
{
    mJsonObject = obj;


    QJsonValue value = obj.value(KDaliNodeName);
    QJsonValue id = obj.value(KDaliNodeId);

    setObjectId((int)id.toDouble());

    QJsonValue children =  obj.value(KDaliNodePropertiesName);
    if (children.isArray()) {
        QJsonArray childrenArray = children.toArray();
        replaceChildren(children.toArray());
    }

}

void TableModel::replaceChildren( QJsonArray array) {
    QJsonArray::iterator iter;

    // first remove rows not in new data
    int rows = mModel->rowCount();
    QString command = " |";
    QList<int> rowsToDelete;
    //reverse order to make it easier to delete by row
    for (int i=rows-1;i>=0;--i) {
        QString currname = mModel->data(mModel->index(i,0), Qt::DisplayRole).toString();
        bool found = false;
        for (iter = array.begin(); iter !=array.end();iter++) {
            QJsonValue val = *iter;
            if (val.isArray()) {
                QJsonArray arr = val.toArray();
                QString nameStr = arr.at(0).toString();
                if (nameStr.compare(currname)==0)
                {
                    found = true;
                    break;
                }
            }
        }
        if (!found) rowsToDelete << i;

    }
    foreach(int row, rowsToDelete)
        mModel->removeRow(row);

    for (iter = array.begin(); iter !=array.end();iter++) {
        QJsonValue val = *iter;
        if (val.isArray()) {
            QJsonArray arr = val.toArray();
            QString nameStr = arr.at(0).toString();
            QString valStr = arr.at(1).toString();
            QStandardItem* name = new QStandardItem(nameStr);
            name->setFlags(name->flags() ^ Qt::ItemIsEditable);
            QStandardItem* value = new QStandardItem(valStr);
            QList<QStandardItem*> items;
            QList<QStandardItem*> old = mModel->findItems(nameStr);
            if (old.empty()) {
                items.append(name);
                items.append(value);
                mModel->appendRow(items);
            } else {
                QModelIndex ind = old.at(0)->index();
                mModel->item(ind.row(),1)->setText(valStr);
            }
            DataObject obj(arr.at(1).toString());
        }
    }
}

