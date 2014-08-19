#ifndef GLOBJECT2_H
#define GLOBJECT2_H

#include <map>
#include <string>
#include <QGLShaderProgram>
#include <QMatrix4x4>

struct progValue {
    enum progValueType {
        VAL_INT,
        VAL_FLOAT,
        VAL_MATRIX44
    };

    progValueType type;

    progValue(float f) {
        type = VAL_FLOAT;
        floatVal = f;
    }

    progValue(int i) {
        type = VAL_INT;
        intVal = i;
    }

    progValue(QMatrix4x4 m) {
        type = VAL_INT;
        matrix4x4Val = m;
    }

    union value {
        float floatVal;
        int intVal;
        QMatrix4x4 matrix4x4Val;
    };
};

class GLObject2
{
public:
    GLObject2();

    bool setProgram(QGLShaderProgram program) {mProgram = program;}
    void setUniform(std::string uniformName, progValue value);

    void drawObject();

private:
    std::map <std::string, int> mAttributeLocations;
    std::map <std::string, int> mUniformLocations;

    QGLShaderProgram mProgram;

};

#endif // GLOBJECT2_H
