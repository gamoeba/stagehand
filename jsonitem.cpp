#include "jsonitem.h"

#include <QIcon>

JsonItem::JsonItem(QJsonObject object)
{
    mObject = object;
    QJsonValue value = mObject.value(QString("Name"));
    QJsonValue id = mObject.value(QString("id"));
    mDisplayName = "("+ QString::number(id.toInt()) + ") " + value.toString() ;
}

QVariant JsonItem::data(int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            return QVariant(mDisplayName);

        case Qt::DecorationRole:
            return QVariant(QIcon(":/stagehand/leaf_visible.png"));
        case JsonRole:
            return QVariant(mObject);
        default:
            return QVariant();

    }

}
