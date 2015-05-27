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
#include <qquaternion.h>
#include <math.h>
#include <map>
#include <algorithm>

const std::string VERTEX_ATTR = "vertex";
const std::string TEXCOORD_ATTR = "texCoord";
const std::string MATRIX_UNIFORM = "viewMatrix";
const std::string SELECTED_UNIFORM = "selected";
const std::string OUTLINE_UNIFORM = "outline";

float fVertices[] = {
    -1, 1, 0.0, 1, 1,0.0,1,-1,0.0,
    -1, -1, 0.0, -1,1,0.0, 1,-1,0.0
};

float fTexCoord[] = {
    0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f,
    0.0f,1.0f, 0.0f,0.0f, 1.0f,1.0f
};


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      mBackgroundColor(0x1A,0x1A,0x34),
      mOutlineColor(0xCC,0xCC,0xff)
{
    mSceneChanged = false;
    mNumberRectangles = 0;
    mScale = 1.0;
    mViewportRatio = 1.0;
    mViewportWidth = 1;
    mViewportHeight = 1;
    mTranslateX = 0;
    mTranslateY = 0;
    mDragX = 0;
    mDragY = 0;
    mSelectionIndex=0;
    mShowScreenShot = false;
    mDragging = false;
    mAnimating = false;

    mAnimatingRotation = false;

    setMouseTracking(true);

    mLogo = QPixmap(":/stagehand/splash.png");

    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(4);
    QGLFormat::setDefaultFormat(glf);
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
    mSceneChanged = true;
}

void GLWidget::clear()
{
    mObjects.clear();
    mSceneChanged = true;
}

void GLWidget::zoomIn()
{
    if (!sceneLoaded()) return;

    if (mAnimating) {
        mScale = mEndScale;
    }
    mAnimationLengthms = 500;
    mStartScale = mScale;
    mEndScale = mScale * 2;
    mElapsedTimer.start();
    mAnimating = true;
    update();
}

void GLWidget::zoomOut()
{
    if (!sceneLoaded()) return;

    if (mAnimating) {
        mAnimating = false;
        mScale = mEndScale;
    }

    mAnimationLengthms = 500;
    mStartScale = mScale;
    mEndScale = mScale / 2.0;
    mElapsedTimer.start();
    mAnimating = true;
    update();
}

void GLWidget::rotateLeft()
{
    if (!sceneLoaded()) return;

    endRotationAnimation();

    mRotationAnimationLengthms = 500;
    mStartRotation = mRotationAngleDegrees;
    mEndRotation = mRotationAngleDegrees-90;
    mRotationTimer.start();
    mAnimatingRotation = true;

    update();
}

void GLWidget::rotateRight()
{
    if (!sceneLoaded()) return;

    endRotationAnimation();

    mRotationAnimationLengthms = 500;
    mStartRotation = mRotationAngleDegrees;
    mEndRotation = mRotationAngleDegrees+90;
    mRotationTimer.start();

    mAnimatingRotation = true;

    update();
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
    double step = mEndScale - mStartScale;
    qint64 elapsedTimems = mElapsedTimer.nsecsElapsed()/1000000;
    double scalefac = ((float)(elapsedTimems)/(float)mAnimationLengthms);
    mScale = mStartScale + scalefac*step;
    if (mElapsedTimer.hasExpired(mAnimationLengthms)) {
        endScaleAnimation();
    }
    update();
}

void GLWidget::animateRotation()
{
    double step = mEndRotation - mStartRotation;
    qint64 elapsedTimens = mRotationTimer.nsecsElapsed();
    double scalefac = (double)elapsedTimens/(double)((double)mRotationAnimationLengthms*1000000.0);

    mRotationAngleDegrees = mStartRotation + scalefac*step;
    if (mRotationTimer.hasExpired(mRotationAnimationLengthms)) {
        endRotationAnimation();
    }
    update();
}

void GLWidget::endScaleAnimation()
{
    if (mAnimating){
        mAnimating = false;
        mScale = mEndScale;
    }
}

void GLWidget::endRotationAnimation()
{
    if (mAnimatingRotation){
        mAnimatingRotation = false;
        mRotationAngleDegrees = mEndRotation;
        mRotationAngleDegrees += 360.0;
        mRotationAngleDegrees = (double)((int)mRotationAngleDegrees % (int)360);
    }
}


void GLWidget::drawScreenshot()
{
    mProgramImage.bind();

    //For the screenshot position, we calculate the destination rectangle that we are targetting for the scene independently
    //This is because the device surface stays fixed, whereas the scene can be moved by the camera
    QMatrix4x4 objectMatrix; //Qt initialises to identity
    applyTranslation(objectMatrix);
    applyRotation(objectMatrix);
    applyAspectRatio(objectMatrix);
    applyScale(objectMatrix);

    glBindTexture(GL_TEXTURE_2D, m_uiTexture);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mScreenShot.width(),
                  mScreenShot.height(),
                  0, GL_BGRA, GL_UNSIGNED_BYTE, mScreenShot.bits());
    mProgramImage.setAttributeArray(VERTEX_ATTR, fVertices, 3);
    mProgramImage.setAttributeArray(TEXCOORD_ATTR, fTexCoord, 2);
    mProgramImage.setUniformValue(MATRIX_UNIFORM, objectMatrix );
    mProgramImage.enableAttributeArray(VERTEX_ATTR, true);
    mProgramImage.enableAttributeArray(TEXCOORD_ATTR, true);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    mProgramImage.enableAttributeArray(VERTEX_ATTR, false);
    mProgramImage.enableAttributeArray(TEXCOORD_ATTR, false);
    mProgramImage.release();
}

bool GLWidget::sceneLoaded()
{
    return mObjects.size()>0;
}

void GLWidget::drawRects(int startRect, int count, bool selected)
{
    QVector3D outline(mOutlineColor.redF(),mOutlineColor.greenF(),mOutlineColor.blueF());
    if (!mShowScreenShot || selected)
    {

        mProgram.bind();
        mProgram.setAttributeArray(VERTEX_ATTR, &mVertices[0], 3);
        mProgram.setAttributeArray(TEXCOORD_ATTR, &mTextureCoords[0], 2);
        mProgram.setUniformValue(MATRIX_UNIFORM, mModelView );
        mProgram.setUniformValue(SELECTED_UNIFORM, selected);
        mProgram.setUniformValue(OUTLINE_UNIFORM, outline);
        mProgram.enableAttributeArray(VERTEX_ATTR, true);
        mProgram.enableAttributeArray(TEXCOORD_ATTR, true);
        glDrawArrays(GL_TRIANGLES, startRect*6, count*6);
        mProgram.enableAttributeArray(VERTEX_ATTR, false);
        mProgram.enableAttributeArray(TEXCOORD_ATTR, false);
        mProgram.release();
    }

    mProgramLines.bind();
    mProgramLines.setAttributeArray(VERTEX_ATTR, &mLines[0], 3);
    mProgramLines.enableAttributeArray(VERTEX_ATTR, true);
    mProgramLines.setUniformValue(MATRIX_UNIFORM, mModelView );
    mProgramLines.setUniformValue(OUTLINE_UNIFORM, outline);

    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, startRect*8, count*8); // lines takes total vertex count
    mProgramLines.enableAttributeArray(VERTEX_ATTR, false);
    mProgramLines.release();
}

void GLWidget::addRect(const QMatrix4x4 matrix,  int objIndex) {

    for (int i=0;i<6;i++) {
        int baseIndex = i*3;
        QVector3D v(fVertices[baseIndex]*0.5f, fVertices[baseIndex+1]*0.5f, fVertices[baseIndex+2]);
        QVector3D res = matrix * v;
        int outputBaseIndex = objIndex *3 * 6 + i*3;
        mVertices[outputBaseIndex] = res[0];
        mVertices[outputBaseIndex+1] = res[1];
        mVertices[outputBaseIndex+2] = res[2];
    }

    int outputBase = objIndex * 3 * 6;
    int lineBase = objIndex * 3 * 8;
    for (int i=0;i<4;i++){
        int outputOffset = i*3;
        mLines[lineBase++] = mVertices[outputBase+outputOffset];
        mLines[lineBase++] = mVertices[outputBase+outputOffset+1];
        mLines[lineBase++] = mVertices[outputBase+outputOffset+2];
        outputOffset = ((i+1)*3) %12; // wrap to start for 8th vertex
        mLines[lineBase++] = mVertices[outputBase+outputOffset];
        mLines[lineBase++] = mVertices[outputBase+outputOffset+1];
        mLines[lineBase++] = mVertices[outputBase+outputOffset+2];
    }

    memcpy(&mTextureCoords[objIndex*2*6], &fTexCoord[0], 12*sizeof(float));
}

void GLWidget::initializeGL ()
{
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glGenTextures(1, &m_uiTexture);



    mProgram.CreateProgram(":/stagehand/shader.vsh",":/stagehand/shader.fsh");
    mProgramLines.CreateProgram(":/stagehand/shaderLines.vsh",":/stagehand/shaderLines.fsh");
    mProgramImage.CreateProgram(":/stagehand/shaderImage.vsh",":/stagehand/shaderImage.fsh");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    mRotationAngleDegrees = 0.0;

}

void GLWidget::resizeGL(int width, int height)
 {
     mViewportRatio = (double)height/(double)width;
     mViewportWidth = width;
     mViewportHeight = height;
     glViewport(0,0,width,height);
}

void GLWidget::buildScene()
{
    if (mObjects.size()>0) {

        std::map<int, SceneObject>::iterator iter;
        int numVerts = mObjects.size();
        mVertices.resize(numVerts*3*6);
        mLines.resize(numVerts*3*8);
        mIds.clear();
        std::fill(mLines.begin(),mLines.end(),0.0f);
        mTextureCoords.resize(numVerts*2*6);
        int objIndex = 0;
        for (iter = mObjects.begin();iter != mObjects.end(); ++iter) {
            SceneObject so = (*iter).second;
            QMatrix4x4 objectMatrix = so.mWorldMatrix;
            QVector3D size = so.mSize;
            if (size.length() > 0.0)
            {
                objectMatrix.scale(size);
                objectMatrix.scale(1.0f, 1.0f, 0.05f);

                addRect(objectMatrix, objIndex);
                mIds[iter->first] = objIndex;
                ++objIndex;
            }
        }
        mNumberRectangles = objIndex;
    }
    mSceneChanged = false;
}

void GLWidget::paintGL()
{
    mBackgroundColor = palette().color(palette().Background);
    mOutlineColor = palette().color(palette().Foreground);
    glClearColor(mBackgroundColor.redF(), mBackgroundColor.greenF(), mBackgroundColor.blueF(), 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    if (mAnimating) {
        animate();
    }
    if (mAnimatingRotation) {
        animateRotation();
    }

    mModelView.setToIdentity();
    applyTranslation(mModelView);
    applyRotation(mModelView);
    applyAspectRatio(mModelView);

    mModelView *= mProjectionMatrix;
    mModelView *= mViewMatrix;
    applyScale(mModelView);

    if (mObjects.size()>0) {
        if (mSceneChanged) {
            buildScene();
        }
        int selectionId = 0;
        if (mSelectionIndex < mSelectedIds.size()) {
            selectionId = mSelectedIds[mSelectionIndex];
        }
        int selectObject = mIds[selectionId];

        if (mShowScreenShot)
            drawScreenshot();
        if (selectObject == 0)
        {
            drawRects(0,mNumberRectangles, false);
        }
        else
        {
            drawRects(0,selectObject, false);
            drawRects(selectObject,1,true);
            drawRects(selectObject+1,mNumberRectangles-selectObject+1,false);
        }
    } else {
        drawLogo();
    }

    glDisable(GL_CULL_FACE);
}

void GLWidget::drawLogo() {
    QMatrix4x4 objectMatrix;
    float ratio = (float)mLogo.height()/(float)mLogo.width();
    objectMatrix.scale(0.5f, ratio*0.5f, 1.0f);

    QImage img = mLogo.toImage();

    glBindTexture(GL_TEXTURE_2D, m_uiTexture);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mLogo.width(),
                  mLogo.height(),
                  0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());


    mProgramImage.bind();
    mProgramImage.setAttributeArray(VERTEX_ATTR, fVertices, 3);
    mProgramImage.setAttributeArray(TEXCOORD_ATTR, fTexCoord, 2);
    mProgramImage.setUniformValue(MATRIX_UNIFORM, objectMatrix );
    mProgramImage.enableAttributeArray(VERTEX_ATTR, true);
    mProgramImage.enableAttributeArray(TEXCOORD_ATTR, true);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    mProgramImage.enableAttributeArray(VERTEX_ATTR, false);
    mProgramImage.enableAttributeArray(TEXCOORD_ATTR, false);
    mProgramImage.release();
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
           objectMatrix.rotate(-mRotationAngleDegrees,0,0,1);
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
    endRotationAnimation();
    endScaleAnimation();

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
    const float scrollFactor = 1.05f;
    float xpos = wheelEvent->posF().x()-(size().width()/2.0f);
    xpos *= mScale;

    float xposzoom = 0.0f;
    if (wheelEvent->delta() < 0.0 ) {
        mScale /= scrollFactor;
        xposzoom = xpos / scrollFactor;
    }  else if (wheelEvent->delta() > 0.0){
        mScale *= scrollFactor;
        xposzoom = xpos * scrollFactor;
    }
    mTranslateX -= xposzoom - xpos;
    repaint();

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
        objectMatrix.rotate(-mRotationAngleDegrees,0,0,1);
        objectMatrix.scale(1.0f, 1.0f, 0.05f);
        QVector3D p1(-0.5, -0.5,0.0);
        QVector3D p2(0.5, -0.5,0.0);
        QVector3D p3(0.5, 0.5,0.0);
        QVector3D p4(-0.5, 0.5,0.0);
        p1 = objectMatrix*p1;
        p2 = objectMatrix*p2;
        p3 = objectMatrix*p3;
        p4 = objectMatrix*p4;
        QVector3D p(x,y, 0.0);

        if (Inside2(p1,p2, p) &&
            Inside2(p2, p3, p) &&
            Inside2(p3, p4, p)  &&
            Inside2(p4, p1, p)) {
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

bool GLWidget::Inside2(const QVector3D &p0, QVector3D &p1, const QVector3D& p2)
{
    // use the formula for the determinant of the triangle of the three points to determine if the points are in a clockwise orientation
    float res = p1.x()*p2.y() - p1.y()*p2.x() -p0.x()*p2.y() + p0.y()*p2.x() + p0.x()*p1.y() - p0.y()*p1.x();
    return res < 0;
}

// transform operations for rendering
void GLWidget::applyTranslation(QMatrix4x4& matrix){
    float transx = (mTranslateX + mDragX);
    float transy = -(mTranslateY + mDragY);
    transx /= mViewportWidth;
    transy /= mViewportHeight;
    matrix.translate(transx ,transy ,0);
}

void GLWidget::applyRotation(QMatrix4x4& matrix){
    matrix.rotate(-mRotationAngleDegrees,0,0,1);
}

void GLWidget::applyAspectRatio(QMatrix4x4& matrix){
    matrix.scale(mAspectRatio * mViewportRatio, 1.0, 1.0);
}

void GLWidget::applyScale(QMatrix4x4& matrix) {
    matrix.scale(mScale,mScale,1.0);
}


