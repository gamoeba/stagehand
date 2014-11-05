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

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QMatrix4x4>

class SceneObject
{
public:
    SceneObject(){mResourceID=-1;}
    SceneObject(QMatrix4x4 worldMatrix, QVector3D size);

    QMatrix4x4 mWorldMatrix;
    QVector3D mSize;
    QVector<int> mChildren;
    int mResourceID;
    QString mName;
};

#endif // SCENEOBJECT_H
