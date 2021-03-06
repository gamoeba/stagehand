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

#include "treemodel.h"

#include <QStandardItemModel>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QApplication>
#include "jsonitem.h"
#include "nodeobject.h"
#include "mainwindow.h"
#include "consts.h"

TreeModel::TreeModel(QObject *parent) :
    QObject(parent)
{
    mModel = new QStandardItemModel;
}

void TreeModel::setTreeData(QJsonDocument doc)
{
    jsonDoc = doc;
    mModel->clear();
    QJsonObject obj = jsonDoc.object();
    QJsonValue value = obj.value(KDaliNodeName);
    QJsonValue id = obj.value(KDaliNodeId);
    NodeObject node(obj, KDaliNodePropertiesName);
    bool visible = node.getProperty(KDaliNodeVisible).toBoolean();
    QStandardItem* itemx = new JsonItem(obj, visible);
    itemx->setEditable(false);
    mModel->appendRow(itemx);
    mIndices[id.toInt()] = itemx;
    QJsonValue children = obj.value(KDaliNodeChildrenName);
    if (children.isArray()) {
        addChildren(itemx, children.toArray(), visible);
    }
}

/*
void TreeModel::setTreeData(Frame* frame)
{
    map<int, SceneObject>& objects = frame->getSceneObjects();
    map<int, SceneObject>::iterator iter;
    SceneObject* root = NULL;
    for (iter = objects.begin();iter !=objects.end();++iter) {
        SceneObject& obj = *iter;
        if (obj.mName.compare("RootLayer")==0)
        {
            root = &obj;
        }
    }
    if (root) {

    }
}
*/

void TreeModel::addChildren(QStandardItem* parent, QJsonArray array, bool visible) {
    QJsonArray::iterator iter;

    for (iter = array.begin(); iter !=array.end();iter++) {
        QJsonValue val = *iter;
        if (val.isObject()) {
            QJsonObject obj = val.toObject();
            QJsonValue value = obj.value(KDaliNodeName);
            QJsonValue id = obj.value(KDaliNodeId);
            int idval = id.toInt();
            NodeObject node(obj, KDaliNodePropertiesName);
            bool nodeVisible = node.getProperty(KDaliNodeVisible).toBoolean();

            if (nodeVisible) {
                nodeVisible = visible;
            }

            QStandardItem* itemx = new JsonItem(obj, nodeVisible);
            itemx->setEditable(false);

            mIndices[idval] = itemx;
            parent->appendRow(itemx);
            QJsonValue children = obj.value(KDaliNodeChildrenName);
            if (children.isArray()) {
                addChildren(itemx, children.toArray(), nodeVisible);
            }
        }
    }
}

QModelIndexList TreeModel::search(const QString &strSearch) {
    QModelIndexList items = mModel->match(
                mModel->index(0, 0),
                Qt::DisplayRole,
                QVariant::fromValue(strSearch),
                -1, // no maximum hit count
                Qt::MatchRecursive|Qt::MatchContains);
    qDebug() << strSearch << " matches: " << items.size();
    return items;
}

