#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QMatrix4x4>

class SceneObject
{
public:
    SceneObject(){;}
    SceneObject(QMatrix4x4 worldMatrix, QVector3D size);

    QMatrix4x4 mWorldMatrix;
    QVector3D mSize;
};

#endif // SCENEOBJECT_H
