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
signals:

public slots:


private:
    QStandardItemModel* mModel;
    QTableView* mTable;
    QJsonObject mJsonObject;
};

#endif // TREEMODEL_H
