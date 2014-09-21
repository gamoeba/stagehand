#ifndef FRAME_H
#define FRAME_H

#include <QMatrix4x4>

#include <string>
#include <list>
#include <map>
#include "sceneobject.h"
#include "treemodel.h"

class Frame
{
public:
    Frame(QJsonDocument doc);
    Frame(Frame& prevFrame, std::list<std::string>& updatedProperties);

    TreeModel& getTreeModel();
    std::map<int, SceneObject>& getSceneObjects();

    QMatrix4x4& getProjectionMatrix() { return mProjectionMatrix; }
    QMatrix4x4& getViewMatrix() {return mViewMatrix;}
    double getAspectRatio() {return mAspectRatio;}

private:
    void addObjects2(QJsonArray array);
    bool addNodeObject(QJsonObject obj);
    void addObjects();



    // source information
    QJsonDocument mDoc;
    std::list<std::string> mUpdatedProperties;
    TreeModel* mTreeModel;
    std::map<int, SceneObject> mObjects;

    QMatrix4x4 mProjectionMatrix;
    QMatrix4x4 mViewMatrix;
    double mAspectRatio;
};

#endif // FRAME_H
