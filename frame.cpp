#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "frame.h"
#include "treemodel.h"
#include "mainwindow.h"
#include "nodeobject.h"


Frame::Frame(QJsonDocument doc)
{
    mDoc = doc;
    mTreeModel = new TreeModel;
    mTreeModel->setTreeData(mDoc);
    addObjects();
}

Frame::Frame(Frame &prevFrame, std::list<std::string> &updatedProperties)
{

}

TreeModel &Frame::getTreeModel()
{

}

std::map<int, SceneObject> &Frame::getSceneObjects()
{

}

void Frame::addObjects() {
    QJsonObject obj = mDoc.object();
    addNodeObject(obj);
    QJsonValue children = obj.value(MainWindow::settings.mNodeChildrenName);
    if (children.isArray()) {
        QJsonArray array = children.toArray();
        addObjects2(array);
    }
}

bool Frame::addNodeObject(QJsonObject obj) {
    QJsonValue value = obj.value(MainWindow::settings.mNodeName);
    QJsonValue id = obj.value(MainWindow::settings.mNodeID);
    int idVal = (int)id.toInt();

    NodeObject node(obj, MainWindow::settings.mNodePropertiesName);
    bool vis = node.getProperty(MainWindow::settings.mNodeVisible).toInt() == 1;

    if (value == MainWindow::settings.mCameraNodeName) {

        DataObject pm = node.getProperty(MainWindow::settings.mPropProjectionMatrixName);
        mProjectionMatrix = pm.get4x4Matrix();
        DataObject vm = node.getProperty(MainWindow::settings.mPropViewMatrixName);
        mViewMatrix = vm.get4x4Matrix();

        double aspectRatio = node.getProperty(MainWindow::settings.mPropAspectRatioName).toDouble();
        mAspectRatio = aspectRatio;
    } else {

        QMatrix4x4 wm = node.getProperty(MainWindow::settings.mPropNodeWorldMatrixName).get4x4Matrix();
        std::vector<double> size = node.getProperty(MainWindow::settings.mPropNodeSizeName).getVector();
        QVector3D qsize(size[0], size[1], size[2]);
        if (vis) {
            //mGLWidget->addObject(idVal, SceneObject(wm, qsize));
        }
    }
    return vis;
}

void Frame::addObjects2(QJsonArray array) {
    QJsonArray::iterator iter;
    QMatrix4x4 projectionMatrix, viewMatrix;


    for (iter = array.begin(); iter !=array.end();iter++) {
        QJsonValue val = *iter;
        if (val.isObject()) {
            QJsonObject obj = val.toObject();
            bool vis = addNodeObject(obj);
            QJsonValue children = obj.value(MainWindow::settings.mNodeChildrenName);
            if (vis && children.isArray()) {
                addObjects2(children.toArray());
            }
        }
    }
}

