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
    void addChildren(QStandardItem* parent, QJsonArray array, bool visible);

    QModelIndex getIndex(int id) {return mIndices[id]->index();}
    QModelIndexList search(const QString& strSearch);
signals:

public slots:

private:
    QStandardItemModel* mModel;
    QJsonDocument jsonDoc;
    std::map<int, QStandardItem*> mIndices;
};

#endif // TREEMODEL_H
