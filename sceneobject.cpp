#include "sceneobject.h"

SceneObject::SceneObject(QMatrix4x4 matrix, QVector3D size)
    : mWorldMatrix(matrix), mSize(size)
{
}
