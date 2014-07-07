/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtOpenGL/qglshaderprogram.h>
#include <QTime>
#include <QVector>
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
    void addObject(int id, SceneObject so, bool selected = false);

    void zoomIn();
    void zoomOut();

    void setScreenShot(QImage img);

    void showScreenShot(bool show){mShowScreenShot=show;update();}

    void setSelection(int id);

signals:
    void selectedId(int id);

protected:
    void paintGL ();
    void initializeGL ();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *eventPress);
    void mouseReleaseEvent(QMouseEvent *releaseEvent);
private:
    GLuint  m_uiTexture;
    qreal   m_fAngle;
    float   m_fScale;
    bool m_showBubbles;
    void paintTexturedCube();

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;

    QMatrix4x4 mProjectionMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mModelView;
    double mAspectRatio;
    std::map<int, SceneObject> mObjects;
    std::vector<int> mSelectedIds;
    int mSelectionIndex;
    bool qtLogo;
    QList<Bubble*> bubbles;
    int frames;
    QTime time;
    QGLShaderProgram program1;

    QGLShaderProgram program2;
    int vertexAttr2;
    int normalAttr2;
    int texCoordAttr2;
    int matrixUniform2;
    int textureUniform2;
    int selectedUniform2;
    int screenShotUniform2;

    QPoint mStartPoint;
    QImage mScreenShot;
    int mTranslateX;
    int mTranslateY;
    int mDragX;
    int mDragY;
    bool mShowScreenShot;
    void select(float x, float y);
};
#endif
