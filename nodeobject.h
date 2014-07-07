#ifndef NODEOBJECT_H
#define NODEOBJECT_H

#include <QJsonObject>
#include <QJsonArray>

#include "dataobject.h"

class NodeObject
{
public:
    NodeObject(QJsonObject obj, QString propertiesFieldName);

    DataObject getProperty(QString propName);

private:
    QJsonObject mObject;
    QJsonArray mProperties;
};

#endif // NOTEOBJECT_H
