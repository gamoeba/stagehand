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

#include "jsonitem.h"
#include "mainwindow.h"
#include "nodeobject.h"

#include <QIcon>

JsonItem::JsonItem(QJsonObject object, bool overallVisible)
{
    mObject = object;
    mOverallVisible = overallVisible;
    QJsonValue value = mObject.value(MainWindow::settings.mNodeName);
    QJsonValue id = mObject.value(MainWindow::settings.mNodeID);
    mDisplayName = "("+ QString::number(id.toInt()) + ") " + value.toString() ;
}

QVariant JsonItem::data(int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            return QVariant(mDisplayName);

        case Qt::DecorationRole:
        {
            NodeObject node(mObject, MainWindow::settings.mNodePropertiesName);
            int isVisible = node.getProperty(MainWindow::settings.mNodeVisible).toInt();
            if (isVisible==1) {
                return QVariant(QIcon(":/stagehand/leaf_visible.png"));
            } else {
                return QVariant(QIcon(":/stagehand/leaf_invisible.png"));
            }
        }
        case Qt::ForegroundRole:
        {
            if (mOverallVisible) {
                return QVariant(QColor(Qt::black));
            } else {
                return QVariant(QColor(Qt::lightGray));
            }
        }
        case JsonRole:
            return QVariant(mObject);
        default:
            return QVariant();

    }

}
