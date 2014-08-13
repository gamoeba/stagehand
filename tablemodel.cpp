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

TableModel::TableModel(QTableView* table) :
    QObject(NULL), mTable(table)
{
    mModel = new QStandardItemModel;
}

void TableModel::setTableData(QJsonObject obj)
{
    mJsonObject = obj;

    mModel->clear();

    QJsonValue value = obj.value(QString("Name"));
    QJsonValue id = obj.value(QString("id"));

    setObjectId((int)id.toDouble());

    QJsonValue children =  obj.value(QString("properties"));
    if (children.isArray()) {
        addChildren(children.toArray());
    }

 /*   QModelIndex mi = mModel->index(1,1);
    mModel->setData(mi, QVariant("test"));

    QStandardItem* item = mModel->itemFromIndex(mi);

    QTableWidget* tw = new QTableWidget(mTable);
    tw->setRowCount(4);
    tw->setColumnCount(4);
    tw->horizontalHeader()->setVisible(false);
    tw->verticalHeader()->setVisible(false);
    tw->setVisible(true);
//            mTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
//            //mTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
//            mTable->verticalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
//            //mTable->verticalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    mTable->setIndexWidget(mi, tw);
*/
}

void TableModel::addChildren( QJsonArray array) {
    QJsonArray::iterator iter;

    for (iter = array.begin(); iter !=array.end();iter++) {
        QJsonValue val = *iter;
        if (val.isArray()) {
            QJsonArray arr = val.toArray();
            QString valStr = arr.at(1).toString();
            //valStr.replace(QRegExp("\\] *, *\\["),"],\n[");
            QStandardItem* name = new QStandardItem(arr.at(0).toString());
            name->setFlags(name->flags() ^ Qt::ItemIsEditable);
            QStandardItem* value = new QStandardItem(valStr);
            QList<QStandardItem*> items;
            items.append(name);
            items.append(value);
            mModel->appendRow(items);

            DataObject obj(arr.at(1).toString());
            obj.isMatrix();


//            QModelIndex mi = mModel->indexFromItem(value);


        }
    }
}

