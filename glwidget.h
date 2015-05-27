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

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtOpenGL/qglshaderprogram.h>
#include <QTime>
#include <QVector>
#include <QTimer>
#include <QElapsedTimer>
#include <map>
#include <vector>

#include "sceneobject.h"
#include "glprogram.h"

class Bubble;
class GLWidget : public QOpenGLWidget {

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

    void rotateRight();
    void rotateLeft();

    void setScreenShot(QImage img);

    void showScreenShot(bool show){mShowScreenShot=show;update();}

    void setSelection(int id);
    QImage getScreenShot();

    void drawScreenshot();

    bool sceneLoaded();
    void setColors(const QColor &backgroundColor, const QColor &outlineColor);
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
    void wheelEvent(QWheelEvent * wheelEvent);
private:
    void buildScene();

    void applyTranslation(QMatrix4x4 &matrix);
    void applyRotation(QMatrix4x4 &matrix);
    void applyAspectRatio(QMatrix4x4 &matrix);
    void applyScale(QMatrix4x4 &matrix);

    void animateRotation();
    void endScaleAnimation();
    void endRotationAnimation();

    bool Inside2(const QVector3D &p0, QVector3D &p1, const QVector3D &p2);

    bool mSceneChanged;

    GLuint  m_uiTexture;
    double   mScale;

    void drawRects(int startRect, int finRect, bool selected);
    void addRect(const QMatrix4x4 matrix, int objIndex);


    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;

    QMatrix4x4 mProjectionMatrix;
    double mRotationAngleDegrees;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mModelView;
    double mAspectRatio;
    double mViewportRatio;
    int mViewportWidth;
    int mViewportHeight;
    std::map<int, SceneObject> mObjects;
    std::vector<int> mSelectedIds;
    std::vector<float> mVertices;
    std::vector<float> mLines;
    std::vector<float> mTextureCoords;
    std::map<int,int> mIds;
    unsigned int mSelectionIndex;
    int mNumberRectangles;
    int frames;
    QTime time;

    GlProgram mProgram;
    GlProgram mProgramLines;
    GlProgram mProgramImage;

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
    QElapsedTimer mElapsedTimer;
    QElapsedTimer mRotationTimer;
    double mStartScale;
    double mEndScale;
    double mStartRotation;
    double mEndRotation;

    qint64 mAnimationLengthms;
    qint64 mRotationAnimationLengthms;
    void drawLogo();

    bool mAnimating;
    bool mAnimatingRotation;


    QColor mBackgroundColor;
    QColor mOutlineColor;

};
#endif
