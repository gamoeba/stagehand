#include "nodeobject.h"
#include "dataobject.h"

NodeObject::NodeObject(QJsonObject obj, QString propertiesFieldName)
{
   mObject = obj;
   mProperties = obj.value(propertiesFieldName).toArray();
}

DataObject NodeObject::getProperty(QString propName)
{
    QJsonArray::iterator iter;

    for (iter=mProperties.begin(); iter != mProperties.end() ; iter++) {
        QJsonArray prop = (*iter).toArray();
        if (prop.at(0).toString() == propName) {
            return DataObject(prop.at(1).toString());
        }
    }
    return DataObject("");
}
