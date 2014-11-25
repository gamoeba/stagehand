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

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtOpenGL/qglshaderprogram.h>
#include <QTime>
#include <QVector>
#include <QTimer>
#include <QElapsedTimer>
#include <map>

#include "sceneobject.h"

class Bubble;
class GLWidget : public QGLWidget {

    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void setAspectRatio(double aspectRatio);
public:
    void setProjectionMatrix(QMatrix4x4 projMatrix);
    void setViewMatrix(QMatrix4x4 viewMatrix);
    void addObject(int id, SceneObject so, bool = false);

    void clear();
    void zoomIn();
    void zoomOut();

    void setScreenShot(QImage img);

    void showScreenShot(bool show){mShowScreenShot=show;update();}

    void setSelection(int id);
    QImage getScreenShot();

public slots:
    void animate();

signals:
    void selectedId(int id);
    void mousePosition(int x, int y);

protected:
    void paintGL ();
    void initializeGL ();
    void resizeGL(int width, int height);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *eventPress);
    void mouseReleaseEvent(QMouseEvent *releaseEvent);
private:
    GLuint  m_uiTexture;
    qreal   m_fAngle;
    float   m_fScale;
    bool m_showBubbles;
    void drawRect();

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;

    QMatrix4x4 mProjectionMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mModelView;
    double mAspectRatio;
    double mViewportRatio;
    std::map<int, SceneObject> mObjects;
    std::vector<int> mSelectedIds;
    unsigned int mSelectionIndex;
    int frames;
    QTime time;

    QGLShaderProgram program;
    int vertexAttr;
    int normalAttr;
    int texCoordAttr;
    int matrixUniform;
    int textureUniform;
    int selectedUniform;
    int screenShotUniform;
    int drawTextureUniform;
    int xthresholdUniform;
    int ythresholdUniform;

    QPoint mStartPoint;
    bool mDragging;
    QImage mScreenShot;
    QPixmap mLogo;
    int mTranslateX;
    int mTranslateY;
    int mDragX;
    int mDragY;
    bool mShowScreenShot;
    void select(float x, float y);
    QTimer mAnimationTimer;
    QElapsedTimer mElapsedTimer;
    float mStartScale;
    float mEndScale;
    qint64 mAnimationLengthms;
    void drawLogo();
};
#endif
