#ifndef JSONITEM_H
#define JSONITEM_H

#include <QStandardItem>
#include <QJsonObject>

class JsonItem : public QStandardItem
{
public:
    static const int JsonRole = Qt::UserRole +1;

    explicit JsonItem(QJsonObject object);
    QVariant data(int role) const;

signals:

public slots:


private:
    QJsonObject mObject;
    QString mDisplayName;
};

#endif // JSONITEM_H
