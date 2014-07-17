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
    bool visible = obj.value(settings.mNodeVisible).toInt()==1;
    QStandardItem* itemx = new JsonItem(obj, visible);
    mModel->appendRow(itemx);
    mIndices[id.toInt()] = itemx;
    QJsonValue children = obj.value(settings.mNodeChildrenName);
    if (children.isArray()) {
        addChildren(itemx, children.toArray(), visible);
    }
}

void TreeModel::addChildren(QStandardItem* parent, QJsonArray array, bool visible) {
    QJsonArray::iterator iter;

    for (iter = array.begin(); iter !=array.end();iter++) {
        QJsonValue val = *iter;
        if (val.isObject()) {
            QJsonObject obj = val.toObject();
            QJsonValue value = obj.value(settings.mNodeName);
            QJsonValue id = obj.value(settings.mNodeID);
            bool nodeVisible = obj.value(settings.mNodeVisible).toInt()==1;
            if (visible) {
                visible = nodeVisible;
            }
            QStandardItem* itemx = new JsonItem(obj, visible);
            mIndices[id.toInt()] = itemx;
            parent->appendRow(itemx);
            QJsonValue children = obj.value(settings.mNodeChildrenName);
            if (children.isArray()) {
                addChildren(itemx, children.toArray(), visible);
            }
        }
    }
}

