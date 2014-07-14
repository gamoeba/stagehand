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

#include "delegate.h"
#include "dataobject.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPainter>

TableDelegate::TableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *TableDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & option,
    const QModelIndex & index ) const
{
    QString str = index.data(Qt::DisplayRole).toString();

    QTableWidget* tw = getTableWidget(str);
    if (tw != NULL) {
        tw->setParent(parent);
        return tw;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void TableDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QTableWidget* tw = qobject_cast<QTableWidget*>(editor);
    if (tw == NULL) {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void TableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QTableWidget* tw = qobject_cast<QTableWidget*>(editor);
    if (tw == NULL) {
        QStyledItemDelegate::setModelData(editor, model, index);
    } else {
        QString str;
        int rows = tw->rowCount();
        int cols = tw->columnCount();
        for (int i = 0 ;i < rows;i++) {
            str += "[";
            for (int j = 0;j < cols;j++) {
                QString val = tw->item(i,j)->data(Qt::DisplayRole).toString();
                str += val;
                if (j < cols-1)
                    str += ",";
            }
            str += "]";
            if (i<rows-1) {
                str += ",";
            }
        }
        if (rows > 1) {
            str = "[" + str + "]";
        }
        model->setData(index,QVariant(str), Qt::EditRole);
    }
}

void TableDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

QSize TableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString str = index.data(Qt::DisplayRole).toString();
    QTableWidget* tw = getTableWidget(str);
    if (tw) {
        int h = tw->rowCount() * tw->rowHeight(0);
        return QSize(option.rect.width(),h);
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

QTableWidget* TableDelegate::getTableWidget(QString str) const{
    DataObject dobj(str);
    if (dobj.isVector() || dobj.isMatrix() || dobj.is4x4Matrix()) {
        QTableWidget* tw = new QTableWidget();
        tw->horizontalHeader()->setHidden(true);
        tw->verticalHeader()->setHidden(true);
        tw->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
        tw->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
        if (dobj.isVector()) {
            std::vector<double> arr = dobj.getVector();
            int cols = arr.size();
            tw->setColumnCount(cols);
            tw->setRowCount(1);
            for (int i=0;i<cols;i++) {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(QString::number(arr[i]));
                tw->setItem(0,i,item);
            }
        } else if (dobj.is4x4Matrix()) {
            QMatrix4x4 matrix = dobj.get4x4Matrix();
            int cols = 4;
            int rows = 4;
            tw->setColumnCount(cols);
            tw->setRowCount(rows);
            for (int i=0;i<rows;i++) {
                for (int j=0;j<cols;j++) {
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::number(matrix(i,j)));
                    tw->setItem(j,i,item);
                }
            }
        }
        tw->resizeRowsToContents();
        tw->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
        tw->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);

        return tw;
    }
    return NULL;
}

void TableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    QString str = index.data(Qt::DisplayRole).toString();
    QTableWidget* tw = getTableWidget(str);
    if (tw) {
        tw->resizeRowsToContents();
        tw->resizeColumnsToContents();
        painter->save();
        painter->translate(option.rect.topLeft());
        tw->render(painter);
        painter->restore();
        delete tw;
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }

}
