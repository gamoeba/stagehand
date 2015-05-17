#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <string>
#include <map>
#include <qglshaderprogram.h>

class GlProgram
{
public:
    GlProgram();

    bool CreateProgram(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc);

    bool bind();
    void release();
    void enableAttributeArray(const std::string& attributeName, bool enable);
    void setAttributeArray(const std::string& attributeName, float* array, int tupleSize, int stride=0);

    void setUniformValue(const std::string& uniformName, const QMatrix4x4 &matrix);

    void setUniformValue(const std::string &uniformName, bool value);
private:
    int lookupUniform(const std::string &uniformName);
    int lookupAttribute(const std::string& attributeName);

    QGLShaderProgram mProgram;
    std::map<std::string, int> mAttributes;
    std::map<std::string, int> mUniforms;
};

#endif // GLPROGRAM_H
