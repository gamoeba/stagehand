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

#include "delegate.h"
#include "dataobject.h"
#include "mainwindow.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPainter>

TableDelegate::TableDelegate(int editableCol, QObject *parent)
    : QStyledItemDelegate(parent), mEditableCol(editableCol)
{
}

QWidget *TableDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & option,
    const QModelIndex & index ) const
{
    QString str = index.data(Qt::DisplayRole).toString();
    if (index.column()!=mEditableCol) {
        return NULL;
    }
    QTableWidget* tw = getTableWidget(str);
    if (tw != NULL) {
        tw->setParent(parent);
        tw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
        int cc = tw->colorCount();
        int w = 0;
        for (int i=0;i<cc;i++)
            w += tw->columnWidth(i);
        int h = 0;
        int rc = tw->rowCount();
        for (int i=0;i<rc;i++)
            h += tw->rowHeight(i);
        return QSize(w,h);
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
        if (dobj.isVector()) {
            std::vector<double> arr = dobj.getVector();
            int cols = arr.size();
            tw->setColumnCount(cols);
            tw->setRowCount(1);
            for (int i=0;i<cols;i++) {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(QString::number(arr[i]));
                item->setTextAlignment(Qt::AlignRight);
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
                    item->setTextAlignment(Qt::AlignRight);
                    tw->setItem(j,i,item);
                }
            }
        }
        QFont font;
        int pointSize = MainWindow::settings.mFontPointSize.toInt();
        font.setPointSize(pointSize);
        tw->setFont(font);
        tw->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tw->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
        tw->setFrameShape(QFrame::StyledPanel);
        tw->setFrameShadow(QFrame::Sunken);
        //tw->setLineWidth(10);
        tw->setGridStyle(Qt::DotLine);
        tw->resizeRowsToContents();
        tw->setEditTriggers(QAbstractItemView::AllEditTriggers);


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

        tw->setStyleSheet("QTableWidget {background-color: transparent;}"
                                        "QHeaderView::section {background-color: transparent;}"
                                        "QHeaderView {background-color: transparent;}"
                                        "QTableCornerButton::section {background-color: transparent;}");

        tw->resize(option.rect.size());


        painter->save();
        painter->translate(option.rect.topLeft());
        tw->render(painter);
        painter->restore();
        delete tw;
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }

}
