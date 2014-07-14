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

