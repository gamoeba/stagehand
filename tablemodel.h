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

#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QTableView>


class TableModel : public QObject
{
    Q_OBJECT
public:
    explicit TableModel(QTableView *table);

    void setTableData(QJsonObject obj);

    QStandardItemModel* model() {return mModel;}
    void addChildren(QJsonArray array);

    int getObjectId() {return mObjectId;}
    void setObjectId(int id) {mObjectId = id;}
signals:

public slots:


private:
    QStandardItemModel* mModel;
    QTableView* mTable;
    QJsonObject mJsonObject;
    int mObjectId;
};

#endif // TREEMODEL_H
