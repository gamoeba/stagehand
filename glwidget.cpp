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

#include "glwidget.h"
#include <QPainter>
#include <QPaintEngine>
#include <QMouseEvent>
#include <math.h>
#include <map>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    m_fScale = 1.0;
    mViewportRatio = 1.0;
    mTranslateX = 0;
    mTranslateY = 0;
    mDragX = 0;
    mDragY = 0;
    mSelectionIndex=0;
    mShowScreenShot = false;
    mDragging = false;

    connect(&mAnimationTimer,SIGNAL(timeout()),SLOT(animate()));
    setMouseTracking(true);

    mLogo = QPixmap(":/stagehand/splash.png");

}

GLWidget::~GLWidget()
{
}

void GLWidget::setProjectionMatrix(QMatrix4x4 projMatrix)
{
    mProjectionMatrix = projMatrix;
}

void GLWidget::setViewMatrix(QMatrix4x4 viewMatrix)
{
    mViewMatrix = viewMatrix;
}

void GLWidget::setAspectRatio(double aspectRatio) {
    mAspectRatio = aspectRatio;
}

void GLWidget::addObject(int id, SceneObject so, bool /*selected*/)
{
    mObjects[id] = so;
}

void GLWidget::clear()
{
    mObjects.clear();
}

void GLWidget::zoomIn()
{
    if (mAnimationTimer.isActive()) {
        mAnimationTimer.stop();
        m_fScale = mEndScale;
        update();
    }
    mAnimationLengthms = 500;
    mStartScale = m_fScale;
    mEndScale = m_fScale * 2;
    mElapsedTimer.start();
    mAnimationTimer.start(10);
}

void GLWidget::zoomOut()
{
    if (mAnimationTimer.isActive()) {
        mAnimationTimer.stop();
        m_fScale = mEndScale;
        update();
    }

    mAnimationLengthms = 500;
    mStartScale = m_fScale;
    mEndScale = m_fScale / 2.0;
    mElapsedTimer.start();
    mAnimationTimer.start(10);
}

void GLWidget::setScreenShot(QImage img)
{
    mScreenShot = img;
}

QImage GLWidget::getScreenShot()
{
    return mScreenShot;
}

void GLWidget::setSelection(int id)
{
    // only update if it isn't the same as is already set
    // this avoids clearing the list when the change was actually
    // caused by this class
    bool updateNeeded = true;
    if (mSelectionIndex < mSelectedIds.size() && mSelectedIds[mSelectionIndex] == id)
    {
        updateNeeded = false;
    }
    if (updateNeeded) {
        mSelectedIds.clear();
        mSelectedIds.push_back(id);
        mSelectionIndex=0;
        update();
    }
}

void GLWidget::animate()
{
    float step = mEndScale - mStartScale;
    qint64 elapsedTimems = mElapsedTimer.nsecsElapsed()/1000000;
    float scalefac = ((float)(elapsedTimems)/(float)mAnimationLengthms);
    m_fScale = mStartScale + scalefac*step;
    if (mElapsedTimer.hasExpired(mAnimationLengthms)) {
        m_fScale = mEndScale;
        mAnimationTimer.stop();
    }
    update();
}

void GLWidget::drawRect()
{
    glBindTexture(GL_TEXTURE_2D, m_uiTexture);
    GLfloat afVertices[] = {
        -0.5, 0.5, 0.5, 0.5,-0.5,0.5,-0.5,-0.5,0.5,
        0.5, -0.5, 0.5, -0.5,0.5,0.5,0.5,0.5,0.5
    };
    program.setAttributeArray(vertexAttr, afVertices, 3);

    GLfloat afTexCoord[] = {
        0.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f,
        1.0f,1.0f, 0.0f,0.0f, 1.0f,0.0f
    };
    program.setAttributeArray(texCoordAttr, afTexCoord, 2);

    GLfloat afNormals[] = {

        0,0,-1, 0,0,-1, 0,0,-1,
        0,0,-1, 0,0,-1, 0,0,-1
    };
    program.setAttributeArray(normalAttr, afNormals, 3);

    program.setUniformValue(textureUniform, 0);    // use texture unit 0

    program.enableAttributeArray(vertexAttr);
    program.enableAttributeArray(normalAttr);
    program.enableAttributeArray(texCoordAttr);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.disableAttributeArray(vertexAttr);
    program.disableAttributeArray(normalAttr);
    program.disableAttributeArray(texCoordAttr);
}

void GLWidget::initializeGL ()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glGenTextures(1, &m_uiTexture);

    QGLShader *vshader = new QGLShader(QGLShader::Vertex);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment);
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    vshader->compileSourceFile(":/stagehand/gles_shader.vsh");
    fshader->compileSourceFile(":/stagehand/gles_shader.fsh");
#else
    vshader->compileSourceFile(":/stagehand/shader.vsh");
    fshader->compileSourceFile(":/stagehand/shader.fsh");
 #endif

    program.addShader(vshader);
    program.addShader(fshader);
    program.link();

    vertexAttr = program.attributeLocation("vertex");
    normalAttr = program.attributeLocation("normal");
    texCoordAttr = program.attributeLocation("texCoord");
    matrixUniform = program.uniformLocation("matrix");
    textureUniform = program.uniformLocation("tex");
    selectedUniform = program.uniformLocation("selected");
    screenShotUniform = program.uniformLocation("screenshot");
    drawTextureUniform = program.uniformLocation("drawTexture");
    xthresholdUniform = program.uniformLocation("xthreshold");
    ythresholdUniform = program.uniformLocation("ythreshold");


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


    m_fAngle = 0;
}

void GLWidget::resizeGL(int width, int height)
 {
     mViewportRatio = (double)height/(double)width;
     glViewport(0,0,width,height);
}

void GLWidget::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);



    if (mObjects.size()>0) {

        m_uiTexture = bindTexture(mScreenShot);
        program.bind();
        mModelView = mProjectionMatrix * mViewMatrix;
        mModelView.translate(mTranslateX + mDragX ,mTranslateY + mDragY ,0);
        mModelView.scale(mAspectRatio * mViewportRatio, 1.0, 1.0);
        mModelView.scale(m_fScale,m_fScale,1.0);

        int selectionId = 0;
        if (mSelectionIndex < mSelectedIds.size()) {
            selectionId = mSelectedIds[mSelectionIndex];
        }

        std::map<int, SceneObject>::iterator iter;
        for (iter = mObjects.begin();iter != mObjects.end();iter++) {
            SceneObject so = (*iter).second;
            QMatrix4x4 objectMatrix = so.mWorldMatrix;

            QVector3D size = so.mSize;
            objectMatrix.scale(size);
            objectMatrix = mModelView * objectMatrix;
            objectMatrix.scale(1.0f, 1.0f, 0.05f);

            QVector3D tl(-0.5, 0.5,0.0);
            QVector3D br(0.5, -0.5,0.0);
            tl = objectMatrix*tl;
            br = objectMatrix*br;
            float xpixels = (float)width() * (br.x()-tl.x())/2.0;
            float xthreshold = 1.0/xpixels;
            float ypixels = (float)height() * (br.y()-tl.y())/2.0;
            float ythreshold = 1.0/ypixels;

            program.setUniformValue(drawTextureUniform, mShowScreenShot && (*iter).first == 1);
            program.setUniformValue(matrixUniform, objectMatrix);
            program.setUniformValue(selectedUniform, (*iter).first == selectionId);
            program.setUniformValue(screenShotUniform, mShowScreenShot);
            program.setUniformValue(xthresholdUniform, xthreshold);
            program.setUniformValue(ythresholdUniform, ythreshold);

            drawRect();

        }
        program.release();
    } else {
        drawLogo();
    }


    glDisable(GL_CULL_FACE);
}

void GLWidget::drawLogo() {
    QMatrix4x4 objectMatrix;
    float ratio = (float)mLogo.height()/(float)mLogo.width();
    objectMatrix.scale(1.0f, -ratio, 1.0f);
    m_uiTexture = bindTexture(mLogo);
    program.bind();
    program.setUniformValue(drawTextureUniform, true);
    program.setUniformValue(matrixUniform, objectMatrix);
    program.setUniformValue(selectedUniform, false);
    program.setUniformValue(screenShotUniform, true);
    program.setUniformValue(xthresholdUniform, 0.2f);
    program.setUniformValue(ythresholdUniform, 0.2f);

    drawRect();
    program.release();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint currpoint = event->pos();
    if (mDragging) {
        currpoint -= mStartPoint;
        mDragX = currpoint.x();
        mDragY = currpoint.y();
        update();
    } else {
        // assume that the first node has the screen size

       if (mObjects.size()>0) {
           float x = (float)event->pos().x()/(float)width()*2.0f -1.0f;
           float y = (float)event->pos().y()/(float)height()*-2.0f + 1.0f;

           SceneObject so = mObjects.begin()->second;

           QMatrix4x4 objectMatrix = so.mWorldMatrix;

           QVector3D size = so.mSize;
           objectMatrix.scale(size);
           objectMatrix = mModelView * objectMatrix;
           objectMatrix.scale(1.0f, 1.0f, 0.05f);
           QVector3D tl(-0.5, 0.5,0.0);
           QVector3D br(0.5, -0.5,0.0);
           tl = objectMatrix*tl;
           br = objectMatrix*br;
           if (x >= tl.x() && y >= tl.y() &&
                   x <= br.x() && y <= br.y()) {
               x-= tl.x();
               y-= tl.y();
               x /= br.x() - tl.x();
               y /= br.y() - tl.y();
               y = 1 - y; //flip axis
               mousePosition((int)(x*size.x()), (int)(y*size.y()));
           } else {
               mousePosition(-1, -1);
           }

        }


    }
}

void GLWidget::mousePressEvent(QMouseEvent *eventPress){
    mStartPoint = eventPress->pos();
    mDragging = true;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *releaseEvent){
    mTranslateX += mDragX;
    mTranslateY += mDragY;
    mDragX = mDragY = 0;

    if (releaseEvent->pos() == mStartPoint) {
        QPoint pos = releaseEvent->pos();
        float x = (float)pos.x()/(float)width()*2.0f -1.0f;
        float y = (float)pos.y()/(float)height()*-2.0f + 1.0f;
        select(x,y);
    }
    mDragging = false;
    update();
}

void GLWidget::wheelEvent(QWheelEvent *wheelEvent)
{
/*    const float scrollFactor = 1.1f;
    float xpos = wheelEvent->posF().x();
    float xposzoom = 0.0f;
    if (wheelEvent->delta() > 0 ) {
        m_fScale /= scrollFactor;
        xposzoom = xpos / scrollFactor;
    }  else if (wheelEvent->delta()<0){
        m_fScale *= scrollFactor;
        xposzoom = xpos * scrollFactor;
    }
    mTranslateX+= xpos - xposzoom;
    repaint();
*/
}

void GLWidget::select(float x, float y) {
    std::vector<int> ids;
    std::map<int, SceneObject>::iterator iter;
    for (iter = mObjects.begin();iter != mObjects.end();iter++) {
        SceneObject so = (*iter).second;
        QMatrix4x4 objectMatrix = so.mWorldMatrix;

        QVector3D size = so.mSize;
        objectMatrix.scale(size);
        objectMatrix = mModelView * objectMatrix;
        objectMatrix.scale(1.0f, 1.0f, 0.05f);
        QVector3D tl(-0.5, 0.5,0.0);
        QVector3D br(0.5, -0.5,0.0);
        tl = objectMatrix*tl;
        br = objectMatrix*br;
        if (x >= tl.x() && y >= tl.y() &&
            x <= br.x() && y <= br.y()) {
            ids.insert(ids.begin(), (*iter).first);
        }

    }
    bool is_equal = false;
    if ( ids.size() > 0 && ids.size() == mSelectedIds.size() )
        is_equal = std::equal ( ids.begin(), ids.end(), mSelectedIds.begin() );

    if (is_equal) {
        mSelectionIndex++;
        mSelectionIndex %= mSelectedIds.size();
    } else {
        mSelectedIds = ids;
        mSelectionIndex = 0;
    }

    if (mSelectionIndex < mSelectedIds.size()) {
        emit selectedId(mSelectedIds[mSelectionIndex]);
    }
}
