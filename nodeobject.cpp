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
