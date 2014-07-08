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

#include "glwidget.h"
#include <QPainter>
#include <QPaintEngine>
#include <QMouseEvent>
#include <math.h>
#include <map>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    qtLogo = true;
    m_fScale = 1.0;
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoBufferSwap(false);
    setMinimumSize(700, 500);

    mTranslateX = 0;
    mTranslateY = 0;
    mDragX = 0;
    mDragY = 0;
    mSelectionIndex=0;
    mShowScreenShot = false;

    connect(&mAnimationTimer,SIGNAL(timeout()),SLOT(animate()));

}

GLWidget::~GLWidget()
{
}

void GLWidget::setProjectionMatrix(QMatrix4x4 projMatrix)
{
    mProjectionMatrix = projMatrix;
    qDebug() << mProjectionMatrix;
}

void GLWidget::setViewMatrix(QMatrix4x4 viewMatrix)
{
    mViewMatrix = viewMatrix;
    qDebug() << mViewMatrix;
}

void GLWidget::setAspectRatio(double aspectRatio) {
    mAspectRatio = aspectRatio;
}

void GLWidget::addObject(int id, SceneObject so, bool selected)
{
    mObjects[id] = so;
}

void GLWidget::zoomIn()
{
    if (mAnimationTimer.isActive()) {
        mAnimationTimer.stop();
        m_fScale = mEndScale;
        update();
    }
    mStartScale = m_fScale;
    mEndScale = m_fScale * 2;
    mSteps = 20;
    mCurrentStep = 0;
    mAnimationTimer.start(30);
}

void GLWidget::zoomOut()
{
    if (mAnimationTimer.isActive()) {
        mAnimationTimer.stop();
        m_fScale = mEndScale;
        update();
    }
    mStartScale = m_fScale;
    mEndScale = m_fScale / 2.0;
    mSteps = 20;
    mCurrentStep = 0;
    mAnimationTimer.start(30);
}

void GLWidget::setScreenShot(QImage img)
{
    mScreenShot = img;
}

void GLWidget::setSelection(int id)
{
    mSelectedIds.clear();
    mSelectedIds.push_back(id);
    mSelectionIndex=0;
    update();
}

void GLWidget::animate()
{
    float step = mEndScale - mStartScale;
    mCurrentStep++;
    m_fScale = mStartScale + ((float)mCurrentStep/(float)mSteps)*step;
    if (mCurrentStep>=mSteps)
        mAnimationTimer.stop();
    update();
}

void GLWidget::paintTexturedCube()
{
    glBindTexture(GL_TEXTURE_2D, m_uiTexture);
    GLfloat afVertices[] = {
        -0.5, 0.5, 0.5, 0.5,-0.5,0.5,-0.5,-0.5,0.5,
        0.5, -0.5, 0.5, -0.5,0.5,0.5,0.5,0.5,0.5 /*,
        -0.5, -0.5, -0.5, 0.5,-0.5,-0.5,-0.5,0.5,-0.5,
        0.5, 0.5, -0.5, -0.5,0.5,-0.5,0.5,-0.5,-0.5,

        0.5, -0.5, -0.5, 0.5,-0.5,0.5,0.5,0.5,-0.5,
        0.5, 0.5, 0.5, 0.5,0.5,-0.5,0.5,-0.5,0.5,
        -0.5, 0.5, -0.5, -0.5,-0.5,0.5,-0.5,-0.5,-0.5,
        -0.5, -0.5, 0.5, -0.5,0.5,-0.5,-0.5,0.5,0.5,

        0.5, 0.5,  -0.5, -0.5, 0.5,  0.5,  -0.5,  0.5,  -0.5,
        -0.5,  0.5,  0.5,  0.5,  0.5,  -0.5, 0.5, 0.5,  0.5,
        -0.5,  -0.5, -0.5, -0.5, -0.5, 0.5,  0.5, -0.5, -0.5,
        0.5, -0.5, 0.5,  0.5,  -0.5, -0.5, -0.5,  -0.5, 0.5
            */
    };
    program2.setAttributeArray(vertexAttr2, afVertices, 3);

    GLfloat afTexCoord[] = {
        0.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f,
        1.0f,1.0f, 0.0f,0.0f, 1.0f,0.0f/*,
        1.0f,1.0f, 1.0f,0.0f, 0.0f,1.0f,
        0.0f,0.0f, 0.0f,1.0f, 1.0f,0.0f,

        1.0f,1.0f, 1.0f,0.0f, 0.0f,1.0f,
        0.0f,0.0f, 0.0f,1.0f, 1.0f,0.0f,
        0.0f,0.0f, 1.0f,1.0f, 1.0f,0.0f,
        1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,

        0.0f,1.0f, 1.0f,0.0f, 1.0f,1.0f,
        1.0f,0.0f, 0.0f,1.0f, 0.0f,0.0f,
        1.0f,0.0f, 1.0f,1.0f, 0.0f,0.0f,
        0.0f,1.0f, 0.0f,0.0f, 1.0f,1.0f*/
    };
    program2.setAttributeArray(texCoordAttr2, afTexCoord, 2);

    GLfloat afNormals[] = {

        0,0,-1, 0,0,-1, 0,0,-1,
        0,0,-1, 0,0,-1, 0,0,-1/*,
        0,0,1, 0,0,1, 0,0,1,
        0,0,1, 0,0,1, 0,0,1,

        -1,0,0, -1,0,0, -1,0,0,
        -1,0,0, -1,0,0, -1,0,0,
        1,0,0, 1,0,0, 1,0,0,
        1,0,0, 1,0,0, 1,0,0,

        0,-1,0, 0,-1,0, 0,-1,0,
        0,-1,0, 0,-1,0, 0,-1,0,
        0,1,0, 0,1,0, 0,1,0,
        0,1,0, 0,1,0, 0,1,0*/
    };
    program2.setAttributeArray(normalAttr2, afNormals, 3);

    program2.setUniformValue(textureUniform2, 0);    // use texture unit 0

    program2.enableAttributeArray(vertexAttr2);
    program2.enableAttributeArray(normalAttr2);
    program2.enableAttributeArray(texCoordAttr2);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    program2.disableAttributeArray(vertexAttr2);
    program2.disableAttributeArray(normalAttr2);
    program2.disableAttributeArray(texCoordAttr2);
}

void GLWidget::initializeGL ()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glGenTextures(1, &m_uiTexture);




    QGLShader *vshader2 = new QGLShader(QGLShader::Vertex);
    const char *vsrc2 =
        "attribute highp vec4 vertex;\n"
        "attribute highp vec4 texCoord;\n"
        "attribute mediump vec3 normal;\n"
        "uniform mediump mat4 matrix;\n"
        "varying highp vec4 texc;\n"
        "varying mediump vec4 color;\n"
        "varying mediump float angle;\n"
        "varying mediump float of;\n"
        "void main(void)\n"
        "{\n"
        "    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
        "    angle = max(dot(normal, toLight), 0.0);\n"
        "    mediump vec4 norm = (vec4(normal, 1.0));\n"
        "    norm = matrix * norm;\n"
        "    of = /*exp*/(dot(norm.rgb, vec3(0.0,0.0,1.0)));\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";
    vshader2->compileSourceCode(vsrc2);

    QGLShader *fshader1 = new QGLShader(QGLShader::Fragment);

    QGLShader *fshader2 = new QGLShader(QGLShader::Fragment);

    const char *fsrc2 =
        "varying highp vec4 texc;\n"
        "int multiplicationFactor = 8;\n"
        "float threshold = 0.01;\n"
        "uniform sampler2D tex;\n"
        "uniform bool selected;\n"
        "uniform bool screenshot;\n"
        "varying mediump float angle;\n"
        "varying mediump float of;\n"

        "void main(void)\n"
        "{\n"

            // multiplicationFactor scales the number of stripes
            "mediump vec2 t = texc.st;\n"

            // the threshold constant defines the with of the lines
            "if (fract(t.s) < threshold  || fract(t.t) < threshold )\n"
                "gl_FragColor = vec4(0.8, 1.0, 1.0, 1.0);\n"
            "else if (fract(t.s) > (1.0 - threshold)  || fract(t.t) > (1.0 - threshold) )\n"
                "gl_FragColor = vec4(0.8, 1.0, 1.0, 1.0);\n"
            "else\n"
                "if (selected){"
                "   gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);\n"

                "}else{ \n"
                   " if (!screenshot)\n"
                   "   gl_FragColor = vec4(0.8, 1.0, 1.0, 0.1);\n"
                "  else\n"
                "       discard;}\n"
        "}\n";
    const char *fsrc1 =
        "varying highp vec4 texc;\n"
        "int multiplicationFactor = 8;\n"
        "float threshold = 0.01;\n"
        "uniform sampler2D tex;\n"
        "uniform bool selected;\n"
        "varying mediump float angle;\n"
        "varying mediump float of;\n"

        "void main(void)\n"
        "{\n"

            // multiplicationFactor scales the number of stripes
            "mediump vec2 t = texc.st;\n"
        "    highp vec4 color = texture2D(tex, texc.st);\n"
        "    gl_FragColor = color;\n"
        "}\n";
    fshader1->compileSourceCode(fsrc1);
    fshader2->compileSourceCode(fsrc2);

    program1.addShader(vshader2);
    program1.addShader(fshader1);
    program1.link();

    program2.addShader(vshader2);
    program2.addShader(fshader2);
    program2.link();

    vertexAttr2 = program2.attributeLocation("vertex");
    normalAttr2 = program2.attributeLocation("normal");
    texCoordAttr2 = program2.attributeLocation("texCoord");
    matrixUniform2 = program2.uniformLocation("matrix");
    textureUniform2 = program2.uniformLocation("tex");
    selectedUniform2 = program2.uniformLocation("selected");
    screenShotUniform2 = program2.uniformLocation("screenshot");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


    m_fAngle = 0;
}

void GLWidget::paintGL()
{
    QPainter painter;
    painter.begin(this);

    painter.beginNativePainting();
    m_uiTexture = bindTexture(mScreenShot);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    mModelView = mProjectionMatrix * mViewMatrix;
    mModelView.translate(mTranslateX + mDragX ,mTranslateY + mDragY ,0);
    mModelView.scale(mAspectRatio, 1.0, 1.0);
    mModelView.scale(m_fScale,m_fScale,1.0);
    //program2.bind();

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

        if (mShowScreenShot && (*iter).first == 1) {
            program1.bind();
        } else {
            program2.bind();
        }
        program2.setUniformValue(matrixUniform2, objectMatrix);
        program2.setUniformValue(selectedUniform2, (*iter).first == selectionId);
        program2.setUniformValue(screenShotUniform2, mShowScreenShot);

        paintTexturedCube();

    }
    program2.release();

    glDisable(GL_CULL_FACE);

    painter.endNativePainting();

    painter.end();

    swapBuffers();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint currpoint = event->pos();
    currpoint -= mStartPoint;
    mDragX = currpoint.x();
    mDragY = currpoint.y();
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *eventPress){
    mStartPoint = eventPress->pos();
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
    update();
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
    if ( ids.size() == mSelectedIds.size() )
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
