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

#include "jsonitem.h"
#include "mainwindow.h"

#include <QIcon>

JsonItem::JsonItem(QJsonObject object, bool overallVisible)
{
    mObject = object;
    mOverallVisible = overallVisible;
    QJsonValue value = mObject.value(MainWindow::settings.mNodeName);
    QJsonValue id = mObject.value(MainWindow::settings.mNodeID);
    QJsonValue isVisible = mObject.value(MainWindow::settings.mNodeVisible);
    mDisplayName = "("+ QString::number(id.toInt()) + ") " + value.toString() ;
}

QVariant JsonItem::data(int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            return QVariant(mDisplayName);

        case Qt::DecorationRole:
        {
            int isVisible = mObject.value(MainWindow::settings.mNodeVisible).toInt();
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
