#include "glprogram.h"
#include <string>

using namespace std;

GlProgram::GlProgram()
{

}


bool GlProgram::CreateProgram(const string& vertexShaderSrc, const string& fragmentShaderSrc)
{
    QGLShader *vshader = new QGLShader(QGLShader::Vertex);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment);

    if (!vshader->compileSourceFile(vertexShaderSrc.c_str()))
        return false;
    if (!fshader->compileSourceFile(fragmentShaderSrc.c_str()))
        return false;

    if (!mProgram.addShader(vshader))
        return false;

    if (!mProgram.addShader(fshader))
        return false;
    return mProgram.link();
}

bool GlProgram::bind()
{
    return mProgram.bind();
}

void GlProgram::release()
{
    mProgram.release();
}

void GlProgram::enableAttributeArray(const std::string& attributeName, bool enable)
{
    int attrIndex = lookupAttribute(attributeName);
    if (enable) {
        mProgram.enableAttributeArray(attrIndex);
    } else {
        mProgram.disableAttributeArray(attrIndex);
    }
}

void GlProgram::setAttributeArray(const string &attributeName, float *array, int tupleSize, int stride)
{
    int attrIndex = lookupAttribute(attributeName);
    mProgram.setAttributeArray(attrIndex, array, tupleSize, stride);
}

void GlProgram::setUniformValue(const string& uniformName, const QMatrix4x4& matrix)
{
    int uniformIndex = lookupUniform(uniformName);
    mProgram.setUniformValue(uniformIndex, matrix);
}

void GlProgram::setUniformValue(const string& uniformName, const QVector3D& vec3)
{
    int uniformIndex = lookupUniform(uniformName);
    mProgram.setUniformValue(uniformIndex, vec3);
}

void GlProgram::setUniformValue(const string &uniformName, const QVector4D &vec4)
{
    int uniformIndex = lookupUniform(uniformName);
    mProgram.setUniformValue(uniformIndex, vec4);
}

void GlProgram::setUniformValue(const string& uniformName, bool value)
{
    int uniformIndex = lookupUniform(uniformName);
    mProgram.setUniformValue(uniformIndex, value);
}

int GlProgram::lookupAttribute(const string &attributeName)
{
    if (mAttributes.count(attributeName)==0) {
        mAttributes[attributeName] = mProgram.attributeLocation(attributeName.c_str());
    }
    return mAttributes[attributeName];
}

int GlProgram::lookupUniform(const string &uniformName)
{
    if (mUniforms.count(uniformName)==0) {
        mUniforms[uniformName] = mProgram.uniformLocation(uniformName.c_str());
    }
    return mUniforms[uniformName];
}
