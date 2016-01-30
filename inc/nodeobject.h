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
    // new method to support the properties names that have changed, match against the new and old value
    DataObject getProperty(QString propName, QString propName2);

private:
    QJsonObject mObject;
    QJsonArray mProperties;
};

#endif // NOTEOBJECT_H
